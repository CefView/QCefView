"use strict";(self.webpackChunkqcefviewdoc=self.webpackChunkqcefviewdoc||[]).push([[332],{3905:function(e,t,a){a.d(t,{Zo:function(){return s},kt:function(){return m}});var n=a(7294);function r(e,t,a){return t in e?Object.defineProperty(e,t,{value:a,enumerable:!0,configurable:!0,writable:!0}):e[t]=a,e}function c(e,t){var a=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);t&&(n=n.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),a.push.apply(a,n)}return a}function i(e){for(var t=1;t<arguments.length;t++){var a=null!=arguments[t]?arguments[t]:{};t%2?c(Object(a),!0).forEach((function(t){r(e,t,a[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(a)):c(Object(a)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(a,t))}))}return e}function o(e,t){if(null==e)return{};var a,n,r=function(e,t){if(null==e)return{};var a,n,r={},c=Object.keys(e);for(n=0;n<c.length;n++)a=c[n],t.indexOf(a)>=0||(r[a]=e[a]);return r}(e,t);if(Object.getOwnPropertySymbols){var c=Object.getOwnPropertySymbols(e);for(n=0;n<c.length;n++)a=c[n],t.indexOf(a)>=0||Object.prototype.propertyIsEnumerable.call(e,a)&&(r[a]=e[a])}return r}var l=n.createContext({}),d=function(e){var t=n.useContext(l),a=t;return e&&(a="function"==typeof e?e(t):i(i({},t),e)),a},s=function(e){var t=d(e.components);return n.createElement(l.Provider,{value:t},e.children)},p={inlineCode:"code",wrapper:function(e){var t=e.children;return n.createElement(n.Fragment,{},t)}},f=n.forwardRef((function(e,t){var a=e.components,r=e.mdxType,c=e.originalType,l=e.parentName,s=o(e,["components","mdxType","originalType","parentName"]),f=d(a),m=r,u=f["".concat(l,".").concat(m)]||f[m]||p[m]||c;return a?n.createElement(u,i(i({ref:t},s),{},{components:a})):n.createElement(u,i({ref:t},s))}));function m(e,t){var a=arguments,r=t&&t.mdxType;if("string"==typeof e||r){var c=a.length,i=new Array(c);i[0]=f;var o={};for(var l in t)hasOwnProperty.call(t,l)&&(o[l]=t[l]);o.originalType=e,o.mdxType="string"==typeof e?e:r,i[1]=o;for(var d=2;d<c;d++)i[d]=a[d];return n.createElement.apply(null,i)}return n.createElement.apply(null,a)}f.displayName="MDXCreateElement"},2266:function(e,t,a){a.r(t),a.d(t,{frontMatter:function(){return o},contentTitle:function(){return l},metadata:function(){return d},toc:function(){return s},default:function(){return f}});var n=a(7462),r=a(3366),c=(a(7294),a(3905)),i=["components"],o={title:"QCefContext"},l='class `QCefContext` <a id="class_q_cef_context" class="anchor"></a>',d={unversionedId:"reference/QCefContext",id:"reference/QCefContext",title:"QCefContext",description:"Represents the CEF context",source:"@site/docs/reference/QCefContext.md",sourceDirName:"reference",slug:"/reference/QCefContext",permalink:"/QCefView/docs/reference/QCefContext",editUrl:"https://github.com/CefView/QCefViewDoc/tree/master/docs/reference/QCefContext.md",tags:[],version:"current",frontMatter:{title:"QCefContext"},sidebar:"default",previous:{title:"QCefConfig",permalink:"/QCefView/docs/reference/QCefConfig"},next:{title:"QCefEvent",permalink:"/QCefView/docs/reference/QCefEvent"}},s=[{value:"Summary",id:"summary",children:[],level:2},{value:"Members",id:"members",children:[{value:'<code>public  </code>QCefContext<code>(QCoreApplication * app,int argc,char ** argv,const </code>QCefConfig<code> * config)</code> <a id="class_q_cef_context_1aa2b8af8d2d806ba8b5110e868d314c8c" class="anchor"></a>',id:"public--qcefcontextqcoreapplication--appint-argcchar--argvconst-qcefconfig--config-",children:[{value:"Parameters",id:"parameters",children:[],level:4}],level:3},{value:'<code>public  </code>~QCefContext<code>()</code> <a id="class_q_cef_context_1a91de7d9c36aafdaca390a355d6da5c6d" class="anchor"></a>',id:"public--qcefcontext-",children:[],level:3},{value:'<code>public void </code>addLocalFolderResource<code>(const QString &amp; path,const QString &amp; url,int priority)</code> <a id="class_q_cef_context_1aecc6f7ee9d296bcf8d2ba470e0c0e454" class="anchor"></a>',id:"public-void-addlocalfolderresourceconst-qstring--pathconst-qstring--urlint-priority-",children:[{value:"Parameters",id:"parameters-1",children:[],level:4}],level:3},{value:'<code>public void </code>addArchiveResource<code>(const QString &amp; path,const QString &amp; url,const QString &amp; password,int priority)</code> <a id="class_q_cef_context_1aba9c70a84379190d151bdc4b634367e6" class="anchor"></a>',id:"public-void-addarchiveresourceconst-qstring--pathconst-qstring--urlconst-qstring--passwordint-priority-",children:[{value:"Parameters",id:"parameters-2",children:[],level:4}],level:3},{value:'<code>public bool </code>addCookie<code>(const QString &amp; name,const QString &amp; value,const QString &amp; domain,const QString &amp; url)</code> <a id="class_q_cef_context_1a6d2e90de7fb5fcf2b7e7a6581d26e62c" class="anchor"></a>',id:"public-bool-addcookieconst-qstring--nameconst-qstring--valueconst-qstring--domainconst-qstring--url-",children:[{value:"Parameters",id:"parameters-3",children:[],level:4},{value:"Returns",id:"returns",children:[],level:4}],level:3},{value:'<code>public const </code>QCefConfig<code>*</code>cefConfig<code>() const</code> <a id="class_q_cef_context_1acfd6416ebc0a8df5cf8961dadeff960e" class="anchor"></a>',id:"public-const-qcefconfigcefconfig-const-",children:[{value:"Returns",id:"returns-1",children:[],level:4}],level:3},{value:'<code>protected bool </code>init<code>(const </code>QCefConfig<code> * config)</code> <a id="class_q_cef_context_1a78836c8d4d2bdf4970a256d8d29c80c6" class="anchor"></a>',id:"protected-bool-initconst-qcefconfig--config-",children:[{value:"Parameters",id:"parameters-4",children:[],level:4},{value:"Returns",id:"returns-2",children:[],level:4}],level:3},{value:'<code>protected void </code>uninit<code>()</code> <a id="class_q_cef_context_1aee74a7460786ddc17f8f9c0f68eaab6b" class="anchor"></a>',id:"protected-void-uninit-",children:[],level:3},{value:'<a id="class_q_cef_context_1a502cdbb18c3abae014e6b8ee42948645" class="anchor"></a>',id:"a-idclass_q_cef_context_1a502cdbb18c3abae014e6b8ee42948645-classanchora",children:[],level:3},{value:'<code>public static </code>QCefContext<code>*</code>instance<code>()</code> <a id="class_q_cef_context_1a3e6491f837fdd72c7b4fefed5569853b" class="anchor"></a>',id:"public-static-qcefcontextinstance-",children:[{value:"Returns",id:"returns-3",children:[],level:4}],level:3}],level:2}],p={toc:s};function f(e){var t=e.components,a=(0,r.Z)(e,i);return(0,c.kt)("wrapper",(0,n.Z)({},p,a,{components:t,mdxType:"MDXLayout"}),(0,c.kt)("h1",{id:"class-qcefcontext-"},"class ",(0,c.kt)("inlineCode",{parentName:"h1"},"QCefContext")," ",(0,c.kt)("a",{id:"class_q_cef_context",class:"anchor"})),(0,c.kt)("pre",null,(0,c.kt)("code",{parentName:"pre"},"class QCefContext\n  : public QObject\n")),(0,c.kt)("p",null,"Represents the CEF context"),(0,c.kt)("h2",{id:"summary"},"Summary"),(0,c.kt)("table",null,(0,c.kt)("thead",{parentName:"table"},(0,c.kt)("tr",{parentName:"thead"},(0,c.kt)("th",{parentName:"tr",align:null},"Members"),(0,c.kt)("th",{parentName:"tr",align:null},"Descriptions"))),(0,c.kt)("tbody",{parentName:"table"},(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"public  "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1aa2b8af8d2d806ba8b5110e868d314c8c"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefContext")),(0,c.kt)("inlineCode",{parentName:"td"},"(QCoreApplication * app,int argc,char ** argv,const "),(0,c.kt)("a",{parentName:"td",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefConfig")),(0,c.kt)("inlineCode",{parentName:"td"}," * config)")),(0,c.kt)("td",{parentName:"tr",align:null},"Constructs the CEF context")),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"public  "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1a91de7d9c36aafdaca390a355d6da5c6d"},(0,c.kt)("inlineCode",{parentName:"a"},"~QCefContext")),(0,c.kt)("inlineCode",{parentName:"td"},"()")),(0,c.kt)("td",{parentName:"tr",align:null},"Destructs the CEF context")),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"public void "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1aecc6f7ee9d296bcf8d2ba470e0c0e454"},(0,c.kt)("inlineCode",{parentName:"a"},"addLocalFolderResource")),(0,c.kt)("inlineCode",{parentName:"td"},"(const QString & path,const QString & url,int priority)")),(0,c.kt)("td",{parentName:"tr",align:null},"Adds a url mapping item with local web resource directory")),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"public void "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1aba9c70a84379190d151bdc4b634367e6"},(0,c.kt)("inlineCode",{parentName:"a"},"addArchiveResource")),(0,c.kt)("inlineCode",{parentName:"td"},"(const QString & path,const QString & url,const QString & password,int priority)")),(0,c.kt)("td",{parentName:"tr",align:null},"Adds a url mapping item with local archive (.zip) file which contains the web resource")),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"public bool "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1a6d2e90de7fb5fcf2b7e7a6581d26e62c"},(0,c.kt)("inlineCode",{parentName:"a"},"addCookie")),(0,c.kt)("inlineCode",{parentName:"td"},"(const QString & name,const QString & value,const QString & domain,const QString & url)")),(0,c.kt)("td",{parentName:"tr",align:null},"Adds a cookie to the CEF context, this cookie is accessible from all browsers created with this context")),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"public const "),(0,c.kt)("a",{parentName:"td",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefConfig")),(0,c.kt)("inlineCode",{parentName:"td"},"*"),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1acfd6416ebc0a8df5cf8961dadeff960e"},(0,c.kt)("inlineCode",{parentName:"a"},"cefConfig")),(0,c.kt)("inlineCode",{parentName:"td"},"() const")),(0,c.kt)("td",{parentName:"tr",align:null},"Gets the ",(0,c.kt)("a",{parentName:"td",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},"QCefConfig"))),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"protected bool "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1a78836c8d4d2bdf4970a256d8d29c80c6"},(0,c.kt)("inlineCode",{parentName:"a"},"init")),(0,c.kt)("inlineCode",{parentName:"td"},"(const "),(0,c.kt)("a",{parentName:"td",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefConfig")),(0,c.kt)("inlineCode",{parentName:"td"}," * config)")),(0,c.kt)("td",{parentName:"tr",align:null},"Initialize the CEF context")),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"protected void "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1aee74a7460786ddc17f8f9c0f68eaab6b"},(0,c.kt)("inlineCode",{parentName:"a"},"uninit")),(0,c.kt)("inlineCode",{parentName:"td"},"()")),(0,c.kt)("td",{parentName:"tr",align:null},"Uninitialize the CEF context")),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null}),(0,c.kt)("td",{parentName:"tr",align:null})),(0,c.kt)("tr",{parentName:"tbody"},(0,c.kt)("td",{parentName:"tr",align:null},(0,c.kt)("inlineCode",{parentName:"td"},"public static "),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefContext")),(0,c.kt)("inlineCode",{parentName:"td"},"*"),(0,c.kt)("a",{parentName:"td",href:"#class_q_cef_context_1a3e6491f837fdd72c7b4fefed5569853b"},(0,c.kt)("inlineCode",{parentName:"a"},"instance")),(0,c.kt)("inlineCode",{parentName:"td"},"()")),(0,c.kt)("td",{parentName:"tr",align:null},"Gets the unique default instance")))),(0,c.kt)("h2",{id:"members"},"Members"),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"public--qcefcontextqcoreapplication--appint-argcchar--argvconst-qcefconfig--config-"},(0,c.kt)("inlineCode",{parentName:"h3"},"public  "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1aa2b8af8d2d806ba8b5110e868d314c8c"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefContext")),(0,c.kt)("inlineCode",{parentName:"h3"},"(QCoreApplication * app,int argc,char ** argv,const "),(0,c.kt)("a",{parentName:"h3",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefConfig")),(0,c.kt)("inlineCode",{parentName:"h3"}," * config)")," ",(0,c.kt)("a",{id:"class_q_cef_context_1aa2b8af8d2d806ba8b5110e868d314c8c",class:"anchor"})),(0,c.kt)("p",null,"Constructs the CEF context"),(0,c.kt)("h4",{id:"parameters"},"Parameters"),(0,c.kt)("ul",null,(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"app")," The application")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"argc")," The argument count")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"argv")," The argument list pointer")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"config")," The ",(0,c.kt)("a",{parentName:"p",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},"QCefConfig")," instance"))),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"public--qcefcontext-"},(0,c.kt)("inlineCode",{parentName:"h3"},"public  "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1a91de7d9c36aafdaca390a355d6da5c6d"},(0,c.kt)("inlineCode",{parentName:"a"},"~QCefContext")),(0,c.kt)("inlineCode",{parentName:"h3"},"()")," ",(0,c.kt)("a",{id:"class_q_cef_context_1a91de7d9c36aafdaca390a355d6da5c6d",class:"anchor"})),(0,c.kt)("p",null,"Destructs the CEF context"),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"public-void-addlocalfolderresourceconst-qstring--pathconst-qstring--urlint-priority-"},(0,c.kt)("inlineCode",{parentName:"h3"},"public void "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1aecc6f7ee9d296bcf8d2ba470e0c0e454"},(0,c.kt)("inlineCode",{parentName:"a"},"addLocalFolderResource")),(0,c.kt)("inlineCode",{parentName:"h3"},"(const QString & path,const QString & url,int priority)")," ",(0,c.kt)("a",{id:"class_q_cef_context_1aecc6f7ee9d296bcf8d2ba470e0c0e454",class:"anchor"})),(0,c.kt)("p",null,"Adds a url mapping item with local web resource directory"),(0,c.kt)("h4",{id:"parameters-1"},"Parameters"),(0,c.kt)("ul",null,(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"path")," The path to the local resource directory")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"url")," The url to be mapped to")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"priority")," The priority"))),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"public-void-addarchiveresourceconst-qstring--pathconst-qstring--urlconst-qstring--passwordint-priority-"},(0,c.kt)("inlineCode",{parentName:"h3"},"public void "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1aba9c70a84379190d151bdc4b634367e6"},(0,c.kt)("inlineCode",{parentName:"a"},"addArchiveResource")),(0,c.kt)("inlineCode",{parentName:"h3"},"(const QString & path,const QString & url,const QString & password,int priority)")," ",(0,c.kt)("a",{id:"class_q_cef_context_1aba9c70a84379190d151bdc4b634367e6",class:"anchor"})),(0,c.kt)("p",null,"Adds a url mapping item with local archive (.zip) file which contains the web resource"),(0,c.kt)("h4",{id:"parameters-2"},"Parameters"),(0,c.kt)("ul",null,(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"path")," The path to the local archive file")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"url")," The url to be mapped to")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"password")," The password of the archive")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"priority")," The priority"))),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"public-bool-addcookieconst-qstring--nameconst-qstring--valueconst-qstring--domainconst-qstring--url-"},(0,c.kt)("inlineCode",{parentName:"h3"},"public bool "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1a6d2e90de7fb5fcf2b7e7a6581d26e62c"},(0,c.kt)("inlineCode",{parentName:"a"},"addCookie")),(0,c.kt)("inlineCode",{parentName:"h3"},"(const QString & name,const QString & value,const QString & domain,const QString & url)")," ",(0,c.kt)("a",{id:"class_q_cef_context_1a6d2e90de7fb5fcf2b7e7a6581d26e62c",class:"anchor"})),(0,c.kt)("p",null,"Adds a cookie to the CEF context, this cookie is accessible from all browsers created with this context"),(0,c.kt)("h4",{id:"parameters-3"},"Parameters"),(0,c.kt)("ul",null,(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"name")," The cookie item name")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"value")," The cookie item value")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"domain")," The applicable domain name")),(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("p",{parentName:"li"},(0,c.kt)("inlineCode",{parentName:"p"},"url")," The applicable url"))),(0,c.kt)("h4",{id:"returns"},"Returns"),(0,c.kt)("p",null,"True on success; otherwise false"),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"public-const-qcefconfigcefconfig-const-"},(0,c.kt)("inlineCode",{parentName:"h3"},"public const "),(0,c.kt)("a",{parentName:"h3",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefConfig")),(0,c.kt)("inlineCode",{parentName:"h3"},"*"),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1acfd6416ebc0a8df5cf8961dadeff960e"},(0,c.kt)("inlineCode",{parentName:"a"},"cefConfig")),(0,c.kt)("inlineCode",{parentName:"h3"},"() const")," ",(0,c.kt)("a",{id:"class_q_cef_context_1acfd6416ebc0a8df5cf8961dadeff960e",class:"anchor"})),(0,c.kt)("p",null,"Gets the ",(0,c.kt)("a",{parentName:"p",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},"QCefConfig")),(0,c.kt)("h4",{id:"returns-1"},"Returns"),(0,c.kt)("p",null,"The ",(0,c.kt)("a",{parentName:"p",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},"QCefConfig")," instance"),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"protected-bool-initconst-qcefconfig--config-"},(0,c.kt)("inlineCode",{parentName:"h3"},"protected bool "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1a78836c8d4d2bdf4970a256d8d29c80c6"},(0,c.kt)("inlineCode",{parentName:"a"},"init")),(0,c.kt)("inlineCode",{parentName:"h3"},"(const "),(0,c.kt)("a",{parentName:"h3",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefConfig")),(0,c.kt)("inlineCode",{parentName:"h3"}," * config)")," ",(0,c.kt)("a",{id:"class_q_cef_context_1a78836c8d4d2bdf4970a256d8d29c80c6",class:"anchor"})),(0,c.kt)("p",null,"Initialize the CEF context"),(0,c.kt)("h4",{id:"parameters-4"},"Parameters"),(0,c.kt)("ul",null,(0,c.kt)("li",{parentName:"ul"},(0,c.kt)("inlineCode",{parentName:"li"},"config")," The ",(0,c.kt)("a",{parentName:"li",href:"/QCefView/docs/reference/QCefConfig#class_q_cef_config"},"QCefConfig")," instance")),(0,c.kt)("h4",{id:"returns-2"},"Returns"),(0,c.kt)("p",null,"True on success; otherwise false"),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"protected-void-uninit-"},(0,c.kt)("inlineCode",{parentName:"h3"},"protected void "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1aee74a7460786ddc17f8f9c0f68eaab6b"},(0,c.kt)("inlineCode",{parentName:"a"},"uninit")),(0,c.kt)("inlineCode",{parentName:"h3"},"()")," ",(0,c.kt)("a",{id:"class_q_cef_context_1aee74a7460786ddc17f8f9c0f68eaab6b",class:"anchor"})),(0,c.kt)("p",null,"Uninitialize the CEF context"),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"a-idclass_q_cef_context_1a502cdbb18c3abae014e6b8ee42948645-classanchora"},(0,c.kt)("a",{id:"class_q_cef_context_1a502cdbb18c3abae014e6b8ee42948645",class:"anchor"})),(0,c.kt)("hr",null),(0,c.kt)("h3",{id:"public-static-qcefcontextinstance-"},(0,c.kt)("inlineCode",{parentName:"h3"},"public static "),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context"},(0,c.kt)("inlineCode",{parentName:"a"},"QCefContext")),(0,c.kt)("inlineCode",{parentName:"h3"},"*"),(0,c.kt)("a",{parentName:"h3",href:"#class_q_cef_context_1a3e6491f837fdd72c7b4fefed5569853b"},(0,c.kt)("inlineCode",{parentName:"a"},"instance")),(0,c.kt)("inlineCode",{parentName:"h3"},"()")," ",(0,c.kt)("a",{id:"class_q_cef_context_1a3e6491f837fdd72c7b4fefed5569853b",class:"anchor"})),(0,c.kt)("p",null,"Gets the unique default instance"),(0,c.kt)("h4",{id:"returns-3"},"Returns"),(0,c.kt)("p",null,"The default instance"))}f.isMDXComponent=!0}}]);