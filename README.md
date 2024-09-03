GPULang
=====

GPULang is a frontend shader langauge meant to serve as a common shader interface for multiple backends. GPULang targets backend platforms directly, by aiming to translate the relatively low level frontend langauge to SPIR-V, DXIL, WGSL and Metal. 

GPULang also offers a reflection API, which allows a title to reason about the shader resource layout, allowing for a dynamic setup of desciptor sets/root signatures/bind groups/etc...

GPULang uses the following libraries (all of which are included), all of which follows either the BSD or GNU lesser general license:

* mcpp
* antlr3c
