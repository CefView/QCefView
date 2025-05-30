/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "QCefView", "index.html", [
    [ "Introduction", "index.html", "index" ],
    [ "Build and Config", "md_docs_201-_build_and_config.html", [
      [ "Build", "md_docs_201-_build_and_config.html#autotoc_md8", [
        [ "Prerequisites", "md_docs_201-_build_and_config.html#autotoc_md9", [
          [ "CMake", "md_docs_201-_build_and_config.html#autotoc_md10", null ],
          [ "Qt Framework", "md_docs_201-_build_and_config.html#autotoc_md11", null ]
        ] ],
        [ "Build QCefView", "md_docs_201-_build_and_config.html#autotoc_md12", null ],
        [ "Additional Notes", "md_docs_201-_build_and_config.html#autotoc_md13", null ]
      ] ],
      [ "Config", "md_docs_201-_build_and_config.html#autotoc_md14", [
        [ "QT_SDK_DIR", "md_docs_201-_build_and_config.html#autotoc_md15", null ],
        [ "CEF_SDK_VERSION", "md_docs_201-_build_and_config.html#autotoc_md16", null ],
        [ "CUSTOM_CEF_SDK_DIR", "md_docs_201-_build_and_config.html#autotoc_md17", null ],
        [ "BUILD_DEMO", "md_docs_201-_build_and_config.html#autotoc_md18", null ],
        [ "BUILD_STATIC", "md_docs_201-_build_and_config.html#autotoc_md19", null ],
        [ "STATIC_CRT", "md_docs_201-_build_and_config.html#autotoc_md20", null ],
        [ "USE_SANDBOX", "md_docs_201-_build_and_config.html#autotoc_md21", null ],
        [ "USE_WIN_DCOMPOSITION", "md_docs_201-_build_and_config.html#autotoc_md22", null ],
        [ "USE_QT_EVENT_LOOP", "md_docs_201-_build_and_config.html#autotoc_md23", null ]
      ] ]
    ] ],
    [ "First Project", "md_docs_202-_first_project.html", [
      [ "Initializing the QCefContext Instance", "md_docs_202-_first_project.html#autotoc_md25", null ],
      [ "Create QCefView Instance", "md_docs_202-_first_project.html#autotoc_md26", null ],
      [ "Create a Simple Web Page", "md_docs_202-_first_project.html#autotoc_md27", null ],
      [ "Run the Application", "md_docs_202-_first_project.html#autotoc_md28", null ]
    ] ],
    [ "Resource Loading", "md_docs_203-_resource_loading.html", [
      [ "Load Online Web Resource", "md_docs_203-_resource_loading.html#autotoc_md30", null ],
      [ "Load Local File With File Path", "md_docs_203-_resource_loading.html#autotoc_md31", null ],
      [ "Add Local Folder Mapping", "md_docs_203-_resource_loading.html#autotoc_md32", null ],
      [ "Add Local Archive Mapping", "md_docs_203-_resource_loading.html#autotoc_md33", null ],
      [ "Use Cases", "md_docs_203-_resource_loading.html#autotoc_md34", null ],
      [ "Summary", "md_docs_203-_resource_loading.html#autotoc_md35", null ]
    ] ],
    [ "Interoperability", "md_docs_204-_interoperability.html", [
      [ "Call C++ from Javascript", "md_docs_204-_interoperability.html#autotoc_md37", [
        [ "Add Javascript Code", "md_docs_204-_interoperability.html#autotoc_md38", null ],
        [ "Add C++ Code", "md_docs_204-_interoperability.html#autotoc_md39", null ],
        [ "Run the Application", "md_docs_204-_interoperability.html#autotoc_md40", null ]
      ] ],
      [ "Call Javascript from C++", "md_docs_204-_interoperability.html#autotoc_md41", [
        [ "Add Javascript Code", "md_docs_204-_interoperability.html#autotoc_md42", null ],
        [ "Add C++ Code", "md_docs_204-_interoperability.html#autotoc_md43", null ],
        [ "Run the Application", "md_docs_204-_interoperability.html#autotoc_md44", null ]
      ] ],
      [ "CefViewQuery", "md_docs_204-_interoperability.html#autotoc_md45", [
        [ "Add Javascript Code", "md_docs_204-_interoperability.html#autotoc_md46", null ],
        [ "Add C++ Code", "md_docs_204-_interoperability.html#autotoc_md47", null ],
        [ "Run the Application", "md_docs_204-_interoperability.html#autotoc_md48", null ]
      ] ]
    ] ],
    [ "Rendering", "md_docs_205-_rendering.html", [
      [ "NCW mode", "md_docs_205-_rendering.html#autotoc_md50", null ],
      [ "OSR mode", "md_docs_205-_rendering.html#autotoc_md51", null ],
      [ "Setting OSR Mode in QCefView", "md_docs_205-_rendering.html#autotoc_md52", null ],
      [ "Hardware Acceleration", "md_docs_205-_rendering.html#autotoc_md53", null ]
    ] ],
    [ "Web APIs", "md_docs_206-_web_a_p_is.html", [
      [ "window.CefViewClient", "md_docs_206-_web_a_p_is.html#CefViewClient", [
        [ "addEventListener(name, listener)", "md_docs_206-_web_a_p_is.html#CefViewClient_addEventListener", [
          [ "Parameters", "md_docs_206-_web_a_p_is.html#autotoc_md55", null ]
        ] ],
        [ "removeEventListener(name, listener)", "md_docs_206-_web_a_p_is.html#CefViewClient_removeEventListener", [
          [ "Parameters", "md_docs_206-_web_a_p_is.html#autotoc_md56", null ]
        ] ],
        [ "invoke(name, ...args)", "md_docs_206-_web_a_p_is.html#CefViewClient_invoke", [
          [ "Parameters", "md_docs_206-_web_a_p_is.html#autotoc_md57", null ]
        ] ]
      ] ],
      [ "window.cefViewQuery", "md_docs_206-_web_a_p_is.html#cefViewQuery", [
        [ "Parameters", "md_docs_206-_web_a_p_is.html#autotoc_md58", [
          [ "Returns", "md_docs_206-_web_a_p_is.html#autotoc_md59", null ]
        ] ]
      ] ],
      [ "window.cefViewQueryCancel", "md_docs_206-_web_a_p_is.html#cefViewQueryCancel", null ]
    ] ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Enumerator", "functions_eval.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_cef_version_8h.html",
"class_q_cef_view.html#abdf0a68139fe9163ecd9b5a0cdeed6d7"
];

var SYNCONMSG = 'click to disable panel synchronization';
var SYNCOFFMSG = 'click to enable panel synchronization';