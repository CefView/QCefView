# Interoperability
[TOC]

QCefView provides the capabilities of interoperability between native context and web context, thus the developer can call Javascript from C++ code easily, vice versa. This allows you to create hybrid applications that combine the power of web technologies with the capabilities of native C++ code.

The approach of the interoperability was implemented by inserting a bridge object into the web context displayed in all the frames and browsers managed by QCefView. The bridge object provides methods for communicating with native code. For more details please refer to the API reference [WebAPIs](docs/06-WebAPIs.md)

> [!tip]
> The bridge object is mounted at window object, and the object name could be configured through the `QCefConfig::setBridgeObjectName`. The default name is [CefViewClient](@ref CefViewClient).

## Call C++ from Javascript

This section describes how to call C++ code from Javascript running within the QCefView. The bridge object provides methods to invoke C++ slots, allowing you to execute native code in response to events or actions in your web application.

The bridge object provides the following method to invoke C++ code from Javascript

[CefViewClient.invoke](@ref CefViewClient_invoke)`(name, ...args)`

When this method gets called from Javascript, the following Qt signal will be emitted

`void QCefView::invokeMethod(int browserId,int frameId,const QString & method,const QVariantList & arguments)`

> [!note]
> The Javascript method [CefViewClient.invoke](@ref CefViewClient_invoke)`(name, ...args)` is `ASYNCHRONOUS` operation, that means the calling from Javascript returns immediately regardless the execution of C++ Qt slot

> [!note]
> The C++ code invoked by Javascript will be executed in the QT_UI thread.

Now let's write a small piece of code to demonstrate the invocation from Javascript to C++.

### Add Javascript Code

First add Javascript code as follows into the \<script\> block
```javascript
    function onInvokeMethodClicked(name, ...arg) {
      // invoke C++ code
      window.CallBridge.invokeMethod(name, ...arg);
    }
```

and add the html code
```html
    <label> Test Case for InvokeMethod </label>
    <br />
    <input
      type="button"
      value="Invoke Method"
      onclick="onInvokeMethodClicked('TestMethod', 1, false, 'arg3')"
    />
```

### Add C++ Code

Then add code in C++ to handle the invocation
```cpp
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  // ...

  // connect the invokeMethod to the slot
  connect(cefViewWidget,
          SIGNAL(invokeMethod(int, int, const QString&, const QVariantList&)),
          this,
          SLOT(onInvokeMethod(int, int, const QString&, const QVariantList&)));
  // ...
}

void
MainWindow::onInvokeMethod(int browserId, int frameId, const QString& method, const QVariantList& arguments)
{
  // extract the arguments and dispatch the invocation to corresponding handler
  if (0 == method.compare("TestMethod")) {
    QMetaObject::invokeMethod(
      this,
      [=]() {
        QString title("QCef InvokeMethod Notify");
        QString text = QString("Current Thread: QT_UI\r\n"
                               "Method: %1\r\n"
                               "Arguments:\r\n")
                         .arg(method);

        for (int i = 0; i < arguments.size(); i++) {
          // clang-format off
          text.append(QString("%1 Type:%2, Value:%3\r\n")
              .arg(i)
              .arg(arguments[i].typeName())
              .arg(arguments[i].toString()));
          // clang-format on
        }

        QMessageBox::information(this->window(), title, text);
      },
      Qt::QueuedConnection);
  } else {
  }
}
```
### Run the Application

Now let's run the application.

![First Project](/img/guide/invoke-method-01.png)

Click the button in web area to invoke the C++ code

![First Project](/img/guide/invoke-method-02.png)


## Call Javascript from C++

This section explains how to call Javascript functions from C++ code. QCefView provides mechanisms to trigger events in the Javascript context, allowing you to update the UI or execute Javascript logic from your native application.

The bridge object provides the following methods to support calling from C++ code to Javascript

- [CefViewClient.addEventListener](@ref CefViewClient_addEventListener)`(name, listener)` 

- [CefViewClient.removeEventListener](@ref CefViewClient_removeEventListener)`(name, listener)`

The developers can add as many event listeners as they want in the Javascript context and trigger the events from C++ code with the following methods

- `public bool QCefView::triggerEvent(const QCefEvent & event)`

- `public bool QCefView::triggerEvent(const QCefEvent & event,int frameId)`

- `public bool QCefView::broadcastEvent(const QCefEvent & event)`

> [!note]
> All the methods above are `ASYNCHRONOUS` operations

Now let's code it

### Add Javascript Code

Add code to the javascript

```javascript
    function onLoad() {
      // Add a event listener to handle the event named 'colorChange'
      Window.CallBridge.addEventListener(
        // event name
        "colorChange"   
        // event handler
        function (color) {
          // change the background color
          document.getElementById("main").style.backgroundColor = color;
        }
      );
    }
```

Add code to the html
```html
<body onload="onLoad()" id="main" class="noselect">
```

### Add C++ Code

Add code to trigger the event be handled in Javascript, here we need to add a button in perform the triggering, we just show the button click action handler slot.

```cpp
void
MainWindow::onBtnChangeColorClicked()
{
  if (cefViewWidget) {
    // create a random color
    QColor color(QRandomGenerator::global()->generate());

    // create the cef event and set the arguments
    QCefEvent event("colorChange");
    event.arguments().append(QVariant::fromValue(color.name(QColor::HexArgb)));

    // broadcast the event to all frames in all browsers created by this QCefView widget
    cefViewWidget->broadcastEvent(event);
  }
}
```

### Run the Application

Now let's run the application.

![First Project](/img/guide/add-event-listener-01.png)

Click the button in native area to invoke the Javascript code

![First Project](/img/guide/add-event-listener-02.png)

## CefViewQuery

In this section, we'll explore how to use `CefViewQuery` to communicate asynchronously between JavaScript and C++ code in your CefView application. This method allows you to send requests from your web page to the native application and receive responses without blocking the user interface.

[window.cefViewQuery](@ref cefViewQuery)`(query)` is yet another approach to communicate from Javascript to C++ code, but in this way the communication is `ASYNCHRONOUS` operation. For more details please refer to the API reference.

> [!note]
> `window.cefViewQuery` operates asynchronously. This means that when you send a query, your JavaScript code doesn't wait for the response. Instead, you provide callback functions (`onSuccess` and `onFailure`) that will be executed when the response arrives.

When this method gets called from Javascript, the following Qt signal will be emitted:

`public void QCefView::cefQueryRequest(int browserId,int frameId,const QCefQuery & query)`

In this section let's demonstrate the usage of CefViewQuery with some simple code.

### Add Javascript Code

Add Javascript code as follows:

```javascript
    function onCallBridgeQueryClicked() {
      var query = {
        request: document.getElementById("message").value,
        onSuccess: function (response) {
          alert("Success: " + response);
        },
        onFailure: function (error_code, error_message) {
          alert("Failure: " + error_message + " (Error Code: " + error_code + ")");
        },
      };
      // ⚠[DEPRECATED]window.CefViewQuery(query);
      window.cefViewQuery(query);
    }
```

Add HTML code as follows:

```html
    <label> Test Case for QCefQuery </label>
    <br />
    <textarea id="message" style="width: 320px; height: 120px">
      this message will be processed by native code.
    </textarea>
    <br />
    <input type="button" value="Query" onclick="onCallBridgeQueryClicked()" />
```

### Add C++ Code

Add C++ code as follows:

```cpp
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  // ...

  // connect the cefQueryRequest to the slot
  connect(cefViewWidget,
          SIGNAL(cefQueryRequest(int, int, const QCefQuery&)),
          this,
          SLOT(onQCefQueryRequest(int, int, const QCefQuery&)));

  // ...
}

void
MainWindow::onQCefQueryRequest(int browserId, int frameId, const QCefQuery& query)
{
  QMetaObject::invokeMethod(
    this,
    [=]() {
      QString title("QCef Query Request");
      QString text = QString("Current Thread: QT_UI\r\n"
                             "Query: %1")
                       .arg(query.request());

      QMessageBox::information(this->window(), title, text);

      QString response = query.request().toUpper();
      query.setResponseResult(true, response);
      cefViewWidget->responseQCefQuery(query);
    },
    Qt::QueuedConnection);
}
```

### Run the Application

Now let's run the application.

![First Project](/img/guide/cefview-query-01.png)

Click the button in web area to invoke the C++ code

![First Project](/img/guide/cefview-query-02.png)