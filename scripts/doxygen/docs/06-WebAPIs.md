# Web APIs
[TOC]

QCefView inserts JavaScript bridge object and methods to `window` object for all browsers frames, with these bridge object you can perform communication between native and web context at anytime

## window.CefViewClient  {#CefViewClient}
This is main bridge object injected to all browser and frames by QCefView. And the object name can be specified by `QCefConfig::setBridgeObjectName`. 

### addEventListener(name, listener)  {#CefViewClient_addEventListener}
Adds a listener for the event with specified name

#### Parameters  
* `name` The event name
* `listener` The listener callback function

### removeEventListener(name, listener)  {#CefViewClient_removeEventListener}
Removes the listener for the event with specified name

#### Parameters  
* `name` The event name
* `listener` The listener callback function

### invoke(name, ...args)  {#CefViewClient_invoke}
Invokes a native method with the specified name and arguments
> [!caution]
> The legacy `invokeMethod(name, ...args)` was deprecated by new method: `invoke(name, ...args)`.（renamed)

#### Parameters
* `name` The method name
* `..args` The arguments for the method


## window.cefViewQuery  {#cefViewQuery}
Sends a cef query request to the native context
> [!caution]
> The legacy `window.CefViewQuery` was deprecated by new method: `window.cefViewQuery`. (uncapitalized)

### Parameters
* `query` The query object


#### Returns
* The query id

## window.cefViewQueryCancel  {#cefViewQueryCancel}
Cancels the query request with the specified id
> [!caution]
> The legacy `window.CefViewQueryCancel` was deprecated by new method: `window.cefViewQueryCancel`. (uncapitalized)

#### Parameters
* `id` The query id



