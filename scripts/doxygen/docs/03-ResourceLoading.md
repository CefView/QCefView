# Resource Loading

To load the web resource, QCefView provide 4 methods to achieve it.

## 1. Load Online Web Resource

This method allows you to load web resources directly from the internet by specifying a URL. It's the simplest way to display online content within your QCefView.

You can pass the full URL of the webpage to the constructor of QCefView.
```cpp
  // build settings for per QCefView
  QCefSetting setting;

  // create the QCefView widget and add it to the layout container
  QCefView* cefView = new QCefView("https://google.com", &setting, nullptr);
```

## 2. Load Local File With File Path

This method enables you to load a local HTML file into the QCefView.  It's useful for displaying static content or for testing web applications locally.

Pass the absolute file path of the web resource file to the constructor of QCefView. Note the file path must be started by "file://" schema.
```cpp
  // build the path to the web resource
  QDir dir = QCoreApplication::applicationDirPath();
  QString webResourceDir = QString("file://") + QDir::toNativeSeparators(dir.filePath("webres/index.html"));

  // build settings for per QCefView
  QCefSetting setting;

  // create the QCefView widget and add it to the layout container
  QCefView* cefView = new QCefView(INDEX_URL, &setting, nullptr);
```
## 3. Add Local Folder Mapping

This method is designed for loading entire web application folders. It maps a local directory to a URL, allowing you to access files within that directory as if they were hosted on a web server. This is particularly useful for Single Page Applications (SPAs) and web applications with complex directory structures.

`public void `[`addLocalFolderResource`](/docs/reference/QCefContext#class_q_cef_context_1aecc6f7ee9d296bcf8d2ba470e0c0e454)`(const QString & path,const QString & url,int priority)`

For example, you build the WebApp project and get the output folder `webres`, the folder structure is as follows:
```bash
full\path\to\webres
                │   index.html
                ├───assets
                ├───docs
                ├───img
```

You can add a mapping item with the following code:
```cpp
 // add a local folder to URL map
  QCefContext::instance()->addLocalFolderResource(
      "full\\path\\to\\webres", 
      "https://domainname"              // This could be any URL you need
      );

  // build settings for per QCefView
  QCefSetting setting;

  // create the QCefView widget and add it to the layout container
  QCefView* cefView = new QCefView(
      "https://domainname/index.html", 
      &setting, 
      this
      );
```

After added the mapping item, you can access all the resource with the URL root appended by the resource relative path.

## 4. Add Local Archive Mapping

This method allows you to load web resources from a ZIP archive. It maps a URL to a ZIP file, enabling you to serve the contents of the archive as if they were a web server directory. This is useful for distributing web applications as a single file or for loading resources from compressed archives.

`public void `[`addArchiveResource`](/docs/reference/QCefContext#class_q_cef_context_1a007272d5df4016143f6b9e221a3b0eb1)`(const QString & path,const QString & url,const QString & password)`

You need to keep the folder structure in the archive file, for example:
```bash
full\path\to\webres.zip
                │   index.html
                ├───assets
                ├───docs
                ├───img
```

You can add a mapping item with the following code:
```cpp
 // add a local zip file to URL map
 QCefContext::instance()->addArchiveResource(
     "full\\path\\to\\webres.zip", 
     "https://domainname",
     "password"                 // pass the password of the zip file if needed
     );

  // build settings for per QCefView
  QCefSetting setting;

  // create the QCefView widget and add it to the layout container
  QCefView* cefView = new QCefView(
      "https://domainname/index.html", 
      &setting, 
      this
      );
```

## Use Cases

The above 3 & 4 are very useful when you develop your WebApp with the popular framework like React, Vue or some others, especially you are building SPA WebApp.

## Summary

These four methods provide flexible options for loading web resources into QCefView, catering to different scenarios from online content to local files and complex web applications. Choose the method that best suits your needs based on the source and structure of your web resources.