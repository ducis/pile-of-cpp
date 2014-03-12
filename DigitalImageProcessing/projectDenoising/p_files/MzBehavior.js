/*---------------------------------------------------------------------------*\
|  Subject:    Html Element behavior base
|  NameSpace:  System.Web.Forms.MzBehavior
|  Author:     meizz
|  Created:    2006-08-05
|  Version:    2006-12-14
|-----------------------------------
|  MSN: huangfr@msn.com QQ:112889082 Copyright (c) meizz
|  http://www.meizz.com/jsframework/ MIT-style license
|  The above copyright notice and this permission notice shall be
|  included in all copies or substantial portions of the Software
\*---------------------------------------------------------------------------*/
//Using("System.Web.Forms.MzEffect");

//op{interval, duration, dynamic}
function MzBehavior()
{
  this.element=MzElement.check(arguments[0]);
  if(!this.element) return;

  this.options=System.extend({
    interval: 20,   //milliseconds
    duration: 360,  //milliseconds
    dynamic:  true
  },arguments[1]||{});

  //prevent repeated behavior
  this.attributeName = "att_"+ this._className.replace(/\W/g, "_");
  if(t=this.element.getAttribute(this.attributeName)) return;
  this.element.setAttribute(this.attributeName, "meizz", 0);

  if("function"==typeof this.initialize) this.initialize();
}
MzBehavior.Extends(System, "MzBehavior");


//op{color, beginColor, backgroundColor, backgroundImage}
MzBehavior.Highlight=function(obj,op)
{
  op=System.extend({backgroundColor: "#D4D0C8"}, op||{});
  MzBehavior.apply(this, [obj, op]);
};
t=MzBehavior.Highlight.Extends(MzBehavior, "MzBehavior.Highlight");

t.initialize=function()
{
  var me=this, op=me.options, obj=me.element;
  op._color = obj.style.color;
  op._backgroundColor = obj.style.backgroundColor;
  op._backgroundImage = obj.style.backgroundImage; if(!op.beginColor){
  if(System.ie) op.beginColor=(obj.currentStyle||obj.style).backgroundColor;
  else op.beginColor = obj.getCssValue("background-color");
  if(!op.beginColor || op.beginColor=="transparent")op.beginColor="#FFFFFF";}
  op.beginColor = MzEffect.formatColor(op.beginColor);

  this.mouseover=function()
  {
    if(me.outer) return;
    if(op.color) obj.style.color=op.color;
    if(op.dynamic)
    {
      MzEffect.highlight(obj, {
        interval:   op.interval,
        duration:   op.duration,
        beginColor: op.beginColor,
        endColor:   op.backgroundColor,
        finalColor: op.backgroundColor,
        onafterfinish: function(e)
        {
          if(op.color) obj.style.color=op.color;
          if(op.backgroundImage) obj.style.backgroundImage=op.backgroundImage;
        }});
    }
    else
    {
      if(op.color)
      obj.style.color=op.color;if(op.backgroundImage)
      obj.style.backgroundImage = op.backgroundImage;
      obj.style.backgroundColor = op.backgroundColor;
    }
    me.outer=true;
  };
  this.mouseout=function()
  {
    if(me.inner) return; obj.style.color=op._color;
    if(op.dynamic)
    {
      MzEffect.highlight(obj, {
        interval:   op.interval,
        duration:   op.duration,
        beginColor: op.backgroundColor,
        endColor:   op.beginColor,
        finalColor: op.beginColor,
        onafterfinish: function(e)
        {
          obj.style.color=op._color;if(op.backgroundImage)
          obj.style.backgroundImage = op._backgroundImage;
          obj.style.backgroundColor = op._backgroundColor;
        }});
    }
    else
    {
      obj.style.color=op._color;if(op.backgroundImage)
      obj.style.backgroundImage = op._backgroundImage;
      obj.style.backgroundColor = op._backgroundColor;
    }
    me.outer=false;
  };
  this.mouseoverHandler=function()
  {
    clearTimeout(me.timer); me.inner=true;
    setTimeout(me.mouseover, 1);
  };
  this.mouseoutHandler =function()
  {
    me.outer=!(me.inner=false);
    me.timer=setTimeout(me.mouseout, 1);
  };
  this.restore=function()
  {
    this.inner=false; this.mouseout();
    obj.removeAttribute(me.attributeName);
    obj.detachEvent("onmouseover",this.mouseoverHandler);
    obj.detachEvent("onmouseout", this.mouseoutHandler);
  };
  obj.attachEvent("onmouseover", this.mouseoverHandler);
  obj.attachEvent("onmouseout",  this.mouseoutHandler);
};









//op{dark,light,color,backgroundColor,backgroundImage,condition(e)}
MzBehavior.Emboss=function(element, op)
{
  op = System.extend({
    dark: "#808080",
    light:"#F5F5F5",
    borderWidth: 1,
    continual: false,
    condition: function(e){return true;}
  },op||{});

  MzBehavior.apply(this, [element, op]);
};
t=MzBehavior.Emboss.Extends(MzBehavior, "MzBehavior.Emboss");
t.initialize=function()
{
  var me=this, obj=this.element; op=this.options, bw = op.borderWidth;
  var borderStyle = bw==1 ? "solid " : "outset ";
  op._color = obj.style.color;
  op._backgroundColor = obj.style.backgroundColor;
  op._backgroundImage = obj.style.backgroundImage;

  this.especial=false;
  /MSIE (\d+(\.\d+)?)/.test(navigator.userAgent);
  if(System.ie)this.especial=parseFloat(RegExp.$1)<7;
  this.especial = this.especial || window.opera;

  if(this.especial)
  {
    var es = obj.currentStyle || obj.style;
    var PT = ((op._PT=es.paddingTop)   =="auto"?0:parseInt(op._PT))+bw;
    var PL = ((op._PL=es.paddingLeft)  =="auto"?0:parseInt(op._PL))+bw;
    var PR = ((op._PR=es.paddingRight) =="auto"?0:parseInt(op._PR))+bw;
    var PB = ((op._PB=es.paddingBottom)=="auto"?0:parseInt(op._PB))+bw;

    var es = obj.style;
    this._pristine=function()
    {
      es.paddingTop =PT +"px"; es.paddingBottom=PB +"px";
      es.paddingLeft=PL +"px"; es.paddingRight =PR +"px";
      es.border="none";   es.color=op._color;
      es.backgroundColor=op._backgroundColor;
      es.backgroundImage=op._backgroundImage;
    };
    this.mouseover=function()
    {
      if(!op.condition(me)){me._pristine(); return;}
      es.borderTop = es.borderLeft    = borderStyle + bw +"px "+ op.light;
      es.borderRight = es.borderBottom= borderStyle + bw +"px "+ op.dark;
      es.paddingTop=(PT-bw) +"px"; es.paddingBottom=(PB-bw) +"px";
      es.paddingLeft=(PL-bw) +"px"; es.paddingRight=(PR-bw) +"px";
      if(op.color) es.color=op.color;
      if(op.backgroundColor) es.backgroundColor=op.backgroundColor;
      if(op.backgroundImage) es.backgroundImage=op.backgroundImage;
    };
    this.mousedown=function()
    {
      if(!op.condition(me)){me._pristine(); return;}
      es.borderTop = es.borderLeft = "inset "+ bw +"px "+ op.dark;
      es.borderRight=es.borderBottom="inset "+ bw +"px "+ op.light;
      es.paddingTop=(PT-bw) +"px"; es.paddingBottom=(PB-bw) +"px";
      es.paddingLeft=(PL-bw) +"px"; es.paddingRight=(PR-bw) +"px";
      if(op.color) es.color=op.color;
      if(op.backgroundColor) es.backgroundColor=op.backgroundColor;
      if(op.backgroundImage) es.backgroundImage=op.backgroundImage;
    };
  }
  else
  {
    var es=obj.style;
    op._BT=es.borderTop;   op._BL=es.borderLeft;
    op._BR=es.borderRight; op._BB=es.borderBottom;
    es.borderTop = es.borderLeft = "solid "+ bw +"px transparent";
    es.borderRight = es.borderBottom = "solid "+ bw +"px transparent";

    this._pristine=function()
    {
      es.borderTop = es.borderLeft = "solid "+ bw +"px transparent";
      es.borderRight = es.borderBottom = "solid "+ bw +"px transparent";
      es.color=op._color;
      es.backgroundColor=op._backgroundColor;
      es.backgroundImage=op._backgroundImage;
    };
    this.mouseover=function()
    {
      if(!op.condition(me)){me._pristine(); return;}
      es.borderTop = es.borderLeft     = borderStyle + bw +"px "+ op.light;
      es.borderRight = es.borderBottom = borderStyle + bw +"px "+ op.dark;
      if(op.color) es.color=op.color;
      if(op.backgroundColor) es.backgroundColor=op.backgroundColor;
      if(op.backgroundImage) es.backgroundImage=op.backgroundImage;
    };
    this.mousedown=function()
    {
      if(!op.condition(me)){me._pristine(); return;}
      es.borderTop = es.borderLeft = "inset "+ bw +"px "+ op.dark;
      es.borderRight = es.borderBottom = "inset "+ bw +"px "+ op.light;
      if(op.color) es.color=op.color;
      if(op.backgroundColor) es.backgroundColor=op.backgroundColor;
      if(op.backgroundImage) es.backgroundImage=op.backgroundImage;
    };
  }

  this._pristine();
  obj.attachEvent("onmouseover", this.mouseover);
  obj.attachEvent("onmouseout",  this._pristine);
  obj.attachEvent("onmousedown", this.mousedown);
  obj.attachEvent("onclick",     this.mouseover);

  this.restore=function()
  {
    if(this.especial)
    {
      es.paddingTop=op._PT; es.paddingBottom=op._PB;
      es.paddingLeft=op._PL; es.paddingRight=op._PR;
    }
    else
    {
      es.borderTop=op._BT; es.borderBottom=op._BB;
      es.borderLeft=op._BL; es.borderRight=op._BR;
    }
    obj.removeAttribute(me.attributeName);
    obj.detachEvent("onmouseover", this.mouseover);
    obj.detachEvent("onmouseout",  this._pristine);
    obj.detachEvent("onmousedown", this.mousedown);
    obj.detachEvent("onclick",     this.mouseover);
  };
};





//2006-11-29
//op{interval,duration,direction,dynamic,continual,width,height,increased,controls,selectedClassName}
MzBehavior.Rotate=function(element, op)
{
  op = System.extend(
  {
    interval: 3000,
    duration: 2000,
    continual: true,
    direction: "random"
  },op||{});
  MzBehavior.apply(this, [element, op]);
};
MzBehavior.Rotate.Extends(MzBehavior, "MzBehavior.Rotate");

MzBehavior.Rotate.prototype.initialize=function()
{
  var me=this, obj=me.element, op=me.options;
  me.interval = op.duration + op.interval;
  me.nodes = [];
  me.timer = null;
  me.activeIndex = 1;
  me.currentIndex = 0;
  obj.style.position = "relative"; //20061208

  for(var i=0, n=obj.childNodes.length; i<n; i++)
  {
    var node = obj.childNodes[i];
    if (node.nodeType==1 && node.tagName) me.nodes.push(node);
  } if (me.nodes.length<=1) return; n=op.controls;
  for(var i=1; i<me.nodes.length; i++) MzElement.hide(me.nodes[i]);
  if("undefined"!=typeof(n)&& n.length&&"object"==typeof(n[0])&&n[0].tagName)
  {
    System.call(me);
    for(var i=0;i<n.length;i++)if(n[i].tagName)n[i].onmouseover=
    new Function("Instance('"+ me.hashCode +"').focus("+ i +")");
    n[me.currentIndex].className=op.selectedClassName||"selected";
  }
  if(this.options.continual) me.timer=
  setTimeout(function(){me.change();}, me.interval);
  setTimeout(function()
  {
    var w = parseFloat(op.width);
    var h = parseFloat(op.height);
    op.width = op.width ? (isNaN(w) ? "" : w) : "";
    op.height= op.height? (isNaN(h) ? "" : h) : "";
    w=parseFloat(System.ie ? obj.currentStyle.width :obj.getCssValue("width"));
    h=parseFloat(System.ie ? obj.currentStyle.height:obj.getCssValue("height"));
    op.width = op.width || (isNaN(w) ? "" : w);
    op.height= op.height|| (isNaN(h) ? "" : h);
    op.width = op.width || me.nodes[0].offsetWidth;
    op.height= op.height|| me.nodes[0].offsetHeight;
    obj.style.width = op.width +"px";
    obj.style.height= op.height+"px";
    obj.style.overflow = "hidden";
  }, 1);
};

MzBehavior.Rotate.prototype.change=function()
{
  if (this.dispatchEvent(new System.Event("onchange"))) this.mask();
  var me = this, op=me.options, n=op.controls;
  if("undefined"!=typeof(n)&& n.length&&"object"==typeof(n[0])&&n[0].tagName)
  {
    for(var i=0; i<n.length; i++) n[i].className = "";
    n[me.currentIndex].className= op.selectedClassName || "selected";
  }
  if(this.options.continual) this.timer=
  setTimeout(function(){me.change();}, me.interval);
};

MzBehavior.Rotate.prototype.mask=function()
{
  var me=this, L=me.nodes.length, I=me.currentIndex, N=me.activeIndex;
  this.currentIndex = N;  this.activeIndex = N+1>=L ? 0 : N+1;

  if(!this.options.dynamic)
  {
    MzElement.hide.apply(this,this.nodes);
    MzElement.show(this.nodes[N]); return;
  }
  if("boolean"==typeof me.options.increased) var B=me.options.increased; else
  var B  = Math.ceil(Math.random()* 1000) % 2 == 0; //true: strengthen
  var maskIndex = B ? N : I, oldIndex  = B ? I : N;

  MzElement.show(me.nodes[I]);
  var region = me.nodes[maskIndex]; MzElement.show(region);

  var originalWidth = region.style.width || "";
  var originalHeight= region.style.height|| "";
  var ow=region.offsetWidth, oh=region.offsetHeight;

  function mm(s){var n=parseFloat(region.getCssValue(s)); return isNaN(n)?0:n;}
  if(!System.ie) //hack for moz opera
  {
    ow -= mm("padding-left");
    ow -= mm("padding-right");
    ow -= mm("border-left-width");
    ow -= mm("border-right-width");
    oh -= mm("padding-top");
    oh -= mm("padding-bottom");
    oh -= mm("border-top-width");
    oh -= mm("border-bottom-width");
  }

  with(region.style)
  {
    zIndex = 1;
    top = left= "0px";
    position = "absolute";
    width = ow +"px";
    height = oh +"px";
  }
  MzElement.show(me.nodes[oldIndex]);

  this.effect = MzEffect.mask(region, 
  {
    trend: B,
    duration: me.options.duration,
    direction: me.options.direction,
    onafterfinish: function(e)
    {
      if(B) MzElement.hide(me.nodes[oldIndex]);
      with(e.element.style)
      {
        position="";zIndex="";
        top = left = "";
        width = originalWidth;
        height = originalHeight;
      }
    }
  });
};
MzBehavior.Rotate.prototype.focus=function(n)
{
  var L=this.nodes.length,I=this.currentIndex;
  if(n>=L) n=L-1; if(L<=1 || n==I) return;
  MzElement.hide.apply(this, this.nodes);
  clearTimeout(this.timer);
  if(this.effect)this.effect.end();
  this.activeIndex=n<0?0:n;this.change();
};


//2006-11-29
//op{interval, duration, direction}
MzBehavior.Marquee=function(element, op)
{
  op = System.extend(
  {
    interval: 3000,
    duration: 2000,
    direction: "random"
  },op||{});
  MzBehavior.apply(this, [element, op]);
};
MzBehavior.Marquee.Extends(MzBehavior, "MzBehavior.Marquee");

MzBehavior.Marquee.prototype.initialize=function()
{
  var me=this, obj=me.element, op=me.options, a=obj.childNodes;

  obj.style.overflow = "hidden";
  var div = document.createElement("DIV");
  obj.insertBefore(div, obj.firstChild);
  div.style.backgroundColor="green";
  for(var i=a.length-1; i>=0; i--) div.insertBefore(a[i], div.firstChild);
};












//[static method]
MzBehavior.highlight=function(e, op){return new MzBehavior.Highlight(e, op);}
MzBehavior.emboss   =function(e, op){return new MzBehavior.Emboss(e, op);}
MzBehavior.rotate   =function(e, op){return new MzBehavior.Rotate(e, op);}
MzBehavior.marquee  =function(e, op){return new MzBehavior.Marquee(e, op);}
