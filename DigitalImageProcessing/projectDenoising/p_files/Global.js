/*-=< base function >=-*/
//HTMLElement.getElementById extend
if(document && !document.getElementById){document.getElementById=function(id){
if(document.all) return document.all(id); return null;}}

/*
function sleep(ms)
{
  var start = new Date().getTime();
  var xh = new XMLHttpRequest();
  while(new Date().getTime()-start<ms)
  {
    try{
      xh.open("GET", "file:///c:\\?mz="+ Math.random(), false);
      xh.send(null);
    }catch(ex){return;}
  }
}

function Sleep(n){var start=new Date().getTime(); //for opera only
while(true) if(new Date().getTime()-start>n) break;}
*/


/*-=< HTMLElement >=-*/
if("undefined"!=typeof HTMLDocument)
{
  if("undefined"==typeof HTMLDocument.prototype.createStyleSheet)
  {
    HTMLDocument.prototype.createStyleSheet=function(url)
    {
      var e;if(/[\w\-\u4e00-\u9fa5]+\.\w+(\?|$)/.test(url))
      {
        e=document.createElement("LINK"); e.type="text/css";
        e.rel="Stylesheet"; e.href=url;}else{
        e=document.createElement("STYLE"); e.type="text/css";
      }
      document.getElementsByTagName("HEAD")[0].appendChild(e);
      return e;
    };
  }
}
function searchByTagName(e, TAG)
{
  while(e!=null && e.tagName){if(e.tagName==TAG.toUpperCase())
  return(e); e=e.parentNode; } return null;
}
function searchById(e, id)
{
  while(e = e.parentNode){if(e.id==id) return(e);} return null;
}
if(typeof(HTMLElement)!="undefined" && !window.opera)
{
  var t=HTMLElement.prototype
  t.contains = function(obj)
  {
    if(obj==this)return true;
    if(obj==null)return false; 
    return this.contains(obj.parentNode); 
  };
  t.__defineGetter__("outerHTML",function()
  {
    var a=this.attributes, str="<"+this.tagName, i=0;for(;i<a.length;i++)
    if(a[i].specified) str+=" "+a[i].name+'="'+a[i].value+'"';
    if(!this.canHaveChildren) return str+" />";
    return str+">"+this.innerHTML+"</"+this.tagName+">";
  });
  t.__defineSetter__("outerHTML",function(s)
  {
    var r = this.ownerDocument.createRange();
    r.setStartBefore(this);
    var df = r.createContextualFragment(s);
    this.parentNode.replaceChild(df, this);
    return s;
  });
  t.__defineGetter__("canHaveChildren",function()
  {
    return !/^(area|base|basefont|col|frame|hr|img|br|input|isindex|link|meta|param)$/.test(this.tagName.toLowerCase());
  });
};
if("undefined"!=typeof HTMLElement)
{
  HTMLElement.prototype.getCssValue=function(prop)
  {
    if(!window.getComputedStyle) return null;
    prop=prop.replace(/([A-Z])/g, "-$1").toLowerCase();
    return window.getComputedStyle(this, "").getPropertyValue(prop);
  };
}
if(!window.attachEvent && window.addEventListener)
{
  Window.prototype.attachEvent = HTMLDocument.prototype.attachEvent=
  HTMLElement.prototype.attachEvent=function(en, func, cancelBubble)
  {
    var cb = cancelBubble ? true : false;
    this.addEventListener(en.toLowerCase().substr(2), func, cb);
  };
  Window.prototype.detachEvent = HTMLDocument.prototype.detachEvent=
  HTMLElement.prototype.detachEvent=function(en, func, cancelBubble)
  {
    var cb = cancelBubble ? true : false;
    this.removeEventListener(en.toLowerCase().substr(2), func, cb);
  };
}
if(typeof Event!="undefined" && !System.ie && !window.opera)
{
  var t=Event.prototype;
  t.__defineSetter__("returnValue", function(b){if(!b)this.preventDefault();  return b;});
  t.__defineSetter__("cancelBubble",function(b){if(b) this.stopPropagation(); return b;});
  t.__defineGetter__("offsetX", function(){return this.layerX;});
  t.__defineGetter__("offsetY", function(){return this.layerY;});
  //t.__defineGetter__("srcElement", function(){var n=this.target; while (n.nodeType!=1)n=n.parentNode;return n;}); 
}

/*-=< Function >=-*/
//apply and call
if(typeof(Function.prototype.apply)!="function")
{
  Function.prototype.apply = function(obj, argu)
  {
    if(obj) obj.constructor.prototype.___caller = this;
    for(var a=[], i=0; i<argu.length; i++) a[i] = "argu["+ i +"]";
    var t = eval((obj ? "obj.___caller" : "this") +"("+ a.join(",") +");");
    if(obj) delete obj.constructor.prototype.___caller; return t;};
    Function.prototype.call = function(obj){
    for(var a=[], i=1; i<arguments.length; i++) a[i-1]=arguments[i];
    return this.apply(obj, a);
  }; 
}

/*-=< Array >=-*/
var t = Array.prototype;
//[extended method] push  insert new item
if(typeof(t.push)!="function")
{
  t.push = function()
  {
    for (var i=0; i<arguments.length; i++)
      this[this.length] = arguments[i];
    return this.length; 
  };
}
//[extended method] shift  delete the first item
if(typeof(t.shift)!="function")
{
  t.shift = function()
  {
    var mm = null;
    if(this.length>0)
    {
      mm = this[0]; for(var i=1; i<this.length; i++)
      this[i-1]=this[i]; this.length=this.length -1;
    }
    return mm;
  };
}
//[extended method] unique  Delete repeated item
t.Unique = function()
{
  for(var a={}, i=0; i<this.length; i++)
  {
    if("undefined"==typeof a[this[i]]) a[this[i]] = 1;
  }
  this.length=0;
  for(i in a) this[this.length] = i; return this;
};
//[extended method] indexOf
if(typeof(t.indexOf)!="function")
{
  t.indexOf=function(item, start)
  {
    start=start||0; if(start<0)start=Math.max(0,this.length+start);
    for(var i=start;i<this.length;i++){if(this[i]===item)return i;}
    return -1;
  };
}
t.include=function(e){return this.indexOf(e)!=-1};
t.remove=function(e)
{
  for(var i=0, a=[]; i<this.length; i++) if(this[i]!=e) a[a.length]=this[i];
  return a;
}

/*-=< Date >=-*/
//datetime format
Date.prototype.format = function(format)
{
  var o = {
    "M+" : this.getMonth()+1, //month
    "d+" : this.getDate(),    //day
    "h+" : this.getHours(),   //hour
    "m+" : this.getMinutes(), //minute
    "s+" : this.getSeconds(), //second
    "q+" : Math.floor((this.getMonth()+3)/3),  //quarter
    "S" : this.getMilliseconds() //millisecond
  }
  if(/(y+)/.test(format)) format=format.replace(RegExp.$1,
    (this.getFullYear()+"").substr(4 - RegExp.$1.length));
  for(var k in o)if(new RegExp("("+ k +")").test(format))
    format = format.replace(RegExp.$1,
      RegExp.$1.length==1 ? o[k] : 
        ("00"+ o[k]).substr((""+ o[k]).length));
  return format;
};

/*-=< Number >=-*/
if(typeof(Number.prototype.toFixed)!="function")
{
    Number.prototype.toFixed = function(d)
    {
        var s=this+"";if(!d)d=0;
        if(s.indexOf(".")==-1)s+=".";s+=new Array(d+1).join("0");
        if (new RegExp("^(-|\\+)?(\\d+(\\.\\d{0,"+ (d+1) +"})?)\\d*$").test(s))
        {
            var s="0"+ RegExp.$2, pm=RegExp.$1, a=RegExp.$3.length, b=true;
            if (a==d+2){a=s.match(/\d/g); if (parseInt(a[a.length-1])>4)
            {
                for(var i=a.length-2; i>=0; i--) {a[i] = parseInt(a[i])+1;
                if(a[i]==10){a[i]=0; b=i!=1;} else break;}
            }
            s=a.join("").replace(new RegExp("(\\d+)(\\d{"+d+"})\\d$"),"$1.$2");
        }if(b)s=s.substr(1);return (pm+s).replace(/\.$/, "");} return this+"";
    };
}
if("undefined"==typeof(encodeURIComponent))encodeURIComponent=function(s){return escape(s);}

/*-=< String >=-*/
var t=String.prototype;
t.trim=function(){return this.replace(/(^[\s\t¡¡]+)|([¡¡\s\t]+$)/g, "");};
t.capitalize=function(){return this.charAt(0).toUpperCase() + this.substr(1);};
t.getByteLength=function(){return this.replace(/[^\x00-\xff]/g, "mm").length;};
t.getAttribute = function(attribute)
{
  if(new RegExp("(^|;)\\s*"+attribute+"\\s*:\\s*([^;]*)\\s*(;|$)","i").test(this))
  return RegExp.$2.replace(/%3B/gi,";").replace(/%25/g,"%"); return null;
};
t.setAttribute = function(attribute, value)
{
  value=(""+value).replace(/%/g,"%25").replace(/;/g,"%3B").replace(/\r|\n/g,"");
  return (attribute +":"+ value +";" + this);
};
t.deleteAttribute = function(attribute)
{
  return this.replace(new RegExp("\\b\\s*"+attribute+"\\s*:\\s*([^;]*)\\s*(;|$)","gi"),"");
};
t.getQueryString = function(name)
{
  var reg = new RegExp("(^|&|\\?)"+ name +"=([^&]*)(&|$)"), r;
  if (r=this.match(reg)) return unescape(r[2]); return null;
};
t.sub = function(n)
{
  var r = /[^\x00-\xff]/g;
  if(this.replace(r, "mm").length <= n) return this;
  n = n - 3;
  var m = Math.floor(n/2);
  for(var i=m; i<this.length; i++)
  {
    if(this.substr(0, i).replace(r, "mm").length>=n)
    {
      return this.substr(0, i) +"...";
    }
  }
  return this;
};
t.format=function()
{
  if(arguments.length==0) return this;
  for(var s=this, i=0; i<arguments.length; i++)
    s=s.replace(new RegExp("\\{"+i+"\\}","g"), arguments[i]);
  return s;
};
String.format=function(str)
{
  if("string"!=typeof(str)||arguments.length<=1) return str;
  for(var i=1; i<arguments.length; i++)
    str=str.replace(new RegExp("\\{"+(i-1)+"\\}","g"), arguments[i]);
  return str;
};

/*-=< Meizz Class >=-*/
//NameSpace: System.MzBrowser
System.MzBrowser=window["MzBrowser"]={};(function()
{
  if(MzBrowser.platform) return;
  var ua = window.navigator.userAgent;
  MzBrowser.platform = window.navigator.platform;

  MzBrowser.firefox = ua.indexOf("Firefox")>0;
  MzBrowser.opera = typeof(window.opera)=="object";
  MzBrowser.ie = !MzBrowser.opera && ua.indexOf("MSIE")>0;
  MzBrowser.mozilla = window.navigator.product == "Gecko";
  MzBrowser.netscape= window.navigator.vendor=="Netscape";
  MzBrowser.safari  = ua.indexOf("safari")>-1&&window.Dom;

  if(MzBrowser.firefox) var re = /Firefox(\s|\/)(\d+(\.\d+)?)/;
  else if(MzBrowser.ie) var re = /MSIE( )(\d+(\.\d+)?)/;
  else if(MzBrowser.opera) var re = /Opera(\s|\/)(\d+(\.\d+)?)/;
  else if(MzBrowser.netscape) var re = /Netscape(\s|\/)(\d+(\.\d+)?)/;
  else if(MzBrowser.mozilla) var re = /rv(\:)(\d+(\.\d+)?)/;

  if("undefined"!=typeof(re)&&re.test(ua))
    MzBrowser.version = parseFloat(RegExp.$2);
})();
//alert(MzBrowser.version);

/*-=< Extend >=-*/
System.loadCssFile=function(cssPath, uniqueId)
{
  if(/\w+\.\w+(\?|$)/.test(cssPath))
  {
    if(!("string"==typeof uniqueId && uniqueId!=""))
    uniqueId = "MzCss_"+ cssPath.replace(/\W/g, "");
    if(document.getElementById(uniqueId)) return;

    var link  = document.createElement("LINK");
    link.href = cssPath;
    link.id   = uniqueId;
    link.type = "text/css";
    link.rel  = "Stylesheet";
    uniqueId  = document.getElementsByTagName("HEAD")[0];
    uniqueId.insertBefore(link, uniqueId.firstChild);
  }
};

var t=window.MzElement=System.MzElement={};
var $=t.check=function(e)
{
  if("object"==typeof e && !e.tagName) return null;
  if("string"==typeof e && !(e=document.getElementById(e)))
  return null; return e;
};
t.hide=function()
{
  for (var e=null, n=arguments.length, i=0; i<n; i++)
  if(e=MzElement.check(arguments[i])) e.style.display="none";
};
t.show=function()
{
  for (var e=null, n=arguments.length, i=0; i<n; i++)
  if(e=MzElement.check(arguments[i])) e.style.display="";
};
t.remove=function()
{
  for (var e=null, n=arguments.length, i=0; i<n; i++)
  if(e=MzElement.check(arguments[i])) e.parentNode.removeChild(e);
};
t.realOffset=function(o)
{
  var e=o, x=y=l=t=0, doc=MzElement.body();
  do{l+=e.offsetLeft||0; t+=e.offsetTop||0; e=e.offsetParent;}while(e);
  do{x+=o.scrollLeft||0; y+=o.scrollTop||0; o=o.parentNode;}while(o);
  return {"x":l-x+doc.scrollLeft, "y":t-y+doc.scrollTop};
};
t.body=function()
{
  var W, H, SL, ST;
  var w=window, d=document, dd=d.documentElement;

  if(w.innerWidth) W=w.innerWidth;
  else if(dd&&dd.clientWidth) W=dd.clientWidth;
  else if(d.body) W=d.body.clientWidth;

  if(w.innerHeight) H=w.innerHeight;
  else if(dd&&dd.clientHeight) H=dd.clientHeight; 
  else if(d.body) H=d.body.clientHeight;

  if(w.pageXOffset) SL=w.pageXOffset;
  else if(dd&&dd.scrollLeft) SL=dd.scrollLeft;
  else if(d.body) SL=d.body.scrollLeft;

  if(w.pageYOffset) ST=w.pageYOffset;
  else if(dd&&dd.scrollTop) ST=dd.scrollTop;
  else if(d.body) ST=d.body.scrollTop;

  return {"scrollTop":ST,"scrollLeft":SL,"clientWidth":W,"clientHeight":H};
}
t.hasClassName=function(element, className)
{
  if(!(element=MzElement.check(element))) return;
  return element.className.split(" ").include(className);
};
t.addClassName=function(element, className)
{
  if(!(element=MzElement.check(element))) return;
  var a=element.className.split(" ");
  if(!a.include(className))a=a.concat(className);
  element.className = a.join(" ").trim(); a=null;
};
t.removeClassName=function(element, className)
{
  if(!(element=MzElement.check(element))) return;
  var r=new RegExp("(^| )"+ className +"( |$)", "g");
  element.className=element.className.replace(r, "$2");
};
