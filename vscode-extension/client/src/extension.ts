/* --------------------------------------------------------------------------------------------
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License. See License.txt in the project root for license information.
 * ------------------------------------------------------------------------------------------ */

import * as net from 'net';

import {
	LanguageClient,
	LanguageClientOptions,
	StreamInfo,
	Trace,
} from 'vscode-languageclient/node';
import { ExtensionContext, workspace } from 'vscode';

let client: LanguageClient;


export function activate(_: ExtensionContext) {

	const connectionInfo = {
		port: 5007,
		host: "localhost"
	};

	const serverOptions = () => {
		const socket = net.connect(connectionInfo);
		const result: StreamInfo = {
			writer: socket,
			reader: socket
		};
		return Promise.resolve(result); 
	};

	// Options to control the language client
	const clientOptions: LanguageClientOptions = {
		// Register the server for plain text documents
		documentSelector: ['gpulang'],
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
