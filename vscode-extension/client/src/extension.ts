/* --------------------------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License. See License.txt in the project root for license information.
 * ------------------------------------------------------------------------------------------ */

import * as net from 'net';
import * as os from 'os';
import * as path from 'path';
import * as vscode from 'vscode';
import * as fs from 'fs';
import { spawn, ChildProcess } from 'child_process';

import {
	LanguageClient,
	LanguageClientOptions,
	StreamInfo,
	Trace,
} from 'vscode-languageclient/node';
import { ExtensionContext, workspace } from 'vscode';
import { rejects } from 'assert';

let client: LanguageClient;
const serverProcess: ChildProcess | null = null;

async function startServerAndConnect(serverPath: string): Promise<void> {
	const serverProcess = spawn(serverPath, [], { stdio: ['ignore', 'pipe', 'pipe'], cwd: path.dirname(serverPath)});

	await new Promise<void>((resolve, reject) => {
		let settled = false;

		serverProcess.stdout.on('data', (data) => {
			if (data.toString().includes('Waiting for client...')) {
				settled = true;
				resolve();
			} else {
				console.log(`Server output: ${data}`);
			}
		});

		serverProcess.on('error', (err) => {
			reject(err);
		});
		serverProcess.on('exit', (code) => {
			reject(new Error(`Server exited with ${code}`));
		});

		setTimeout(() => {
			if (settled) {return;}
			serverProcess.kill();
			reject(new Error('Server did not start in time. Make sure the server path is correct.'));
		}, 500);
	});
}

export function activate(context: ExtensionContext) {
	const serverOptions = async () => {
		
		let platform = '';
		if (os.platform() == "win32")
		{
			platform = "windows";
		}
		else if (os.platform() == "linux")
		{
			platform = "linux";
		}
		else if (os.platform() == "darwin")
		{
			platform = "macos";
		}

		let arch = '';
		if (os.arch() == "x64")
		{
			arch = "x64";
		}
		else if (os.arch() == "arm64")
		{
			arch = "aarch64";
		}

		const binary_name = `gpulang_server-${platform}-${arch}`;

		console.log(platform);
		let serverPath = context.asAbsolutePath(path.join(`bin/`, process.platform === 'win32' ? `${binary_name}.exe` : binary_name));
		if (fs.existsSync(serverPath) === false) {
			// Try loading a local version of it
			serverPath = context.asAbsolutePath(path.join(`bin/`, process.platform === 'win32' ? 'gpulang_server.exe' : 'gpulang_server'));
		}
		await startServerAndConnect(serverPath).catch(err => {
			vscode.window.showErrorMessage(`Failed to GPULang server: ${err.message}`);
			return rejects(err);
		});
		vscode.window.showInformationMessage(`GPULang server started: ${serverPath}`);

		let socket_file;
		if (process.platform === 'win32')
		{
			socket_file = "\\\\.\\pipe\\gpulang_socket";
		}
		else
		{
			socket_file = path.join(os.tmpdir(), "gpulang_socket");
		}

		const socket = net.createConnection(socket_file);
		const result: StreamInfo = {
			writer: socket,
			reader: socket
		};
		return Promise.resolve(result); 
	};

	// Options to control the language client
	const workspaceFolders = vscode.workspace.workspaceFolders;

	const configPath = path.join(workspaceFolders[0].uri.fsPath, 'gpulang_config.json');

	let fd = null;
	if (fs.existsSync(configPath) === false) {
		vscode.window.showErrorMessage(`Could not find gpulang configuration file: ${configPath}`);
	} else {
		fd = fs.openSync(configPath, 'r');
		vscode.window.showInformationMessage(`Using gpulang configuration file: ${configPath}`);
	}

	const fileData = fs.readFileSync(fd);
	const clientOptions: LanguageClientOptions = {
		// Register the server for plain text documents
		documentSelector: ['gpulang'],
		initializationOptions: {
			root : workspaceFolders[0].uri.fsPath,
			config : JSON.parse(fileData.toString('utf8'))
		},
		synchronize: {
			// Notify the server about file changes to '.gpul files contained in the workspace
			fileEvents: workspace.createFileSystemWatcher('**/*.{gpul,gpuh}')
		},
	};

	// Create the language client and start the client.
	client = new LanguageClient(
		'GPULang Language Server', 
		serverOptions,
		clientOptions
	);

	context.subscriptions.push({
        dispose() {
            if (client) {
                client.stop();
            }
            if (serverProcess) {
                serverProcess.kill();
            }
        }
    });

	client.setTrace(Trace.Verbose);

	// Start the client. This will also launch the server
	client.start();
}

export function deactivate(): Thenable<void> | undefined {
	if (!client) {
		return undefined;
	}
	return client.stop();
}
