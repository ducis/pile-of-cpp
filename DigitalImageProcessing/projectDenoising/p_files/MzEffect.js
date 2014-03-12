/*---------------------------------------------------------------------------*\
|  Subject:    Html Element effect base
|  NameSpace:  System.Web.Forms.MzEffect
|  Author:     meizz
|  Created:    2006-07-07
|  Version:    2006-12-12
|-----------------------------------
|  MSN: huangfr@msn.com QQ:112889082 Copyright (c) meizz
|  http://www.meizz.com/jsframework/ MIT-style license
|  The above copyright notice and this permission notice shall be
|  included in all copies or substantial portions of the Software
\*---------------------------------------------------------------------------*/

/*/Interface
Required: initialize() render() finish() restore()
abort() end() cancel() dispose()
op.onbeforestart() op.onbeforeupdate() op.onafterupdate() op.onafterfinish()
/*/
//op{interval, duration, overlap, trend, continual}
function MzEffect()
{
  System.call(this);this.readyState=1;
  this.element=MzElement.check(arguments[0]);
  if(!this.element) return; var t;

  this.terminative=false;  //true for terminate effect

  var op=this.options=System.extend({
    "interval": 10,   //milliseconds
    "duration": 800,  //milliseconds
    "overlap":  false,
    "continual": true,
    "trend": "strengthen" //strengthen|weaken true|false
  },
    arguments[1]||{}
  );
  op["trend"]= ("boolean"==typeof(op["trend"]) && op["trend"]) ||
    ("string"==typeof(op["trend"]) && op["trend"].toLowerCase()=="strengthen");

  //prevent repeated effect
  if(!op.overlap){
  this.attribName = "att_"+ this._className.replace(/\W/g, "_");
  if(t=this.element.getAttribute(this.attribName))if(t=Instance(t))t.cancel();
  this.element.setAttribute(this.attribName, this.hashCode, 0);}

  this.beginTime = new Date().getTime();
  this.endTime = this.beginTime + op.duration;
  if(this.initialize) this.initialize();
  if(op.onbeforestart) op.onbeforestart(this); this.readyState=2;
  if(op.continual) this._process();else if(!this.restore){
  this.restore=function(){};this.dispose();}
};
t=MzEffect.Extends(System, "MzEffect");

t._process=function()
{
  var now = new Date().getTime(), me = this, op=this.options;
  if (now>=this.endTime)
  {
    if(this.render) this.render((op.trend==true ? 1 : 0));
    if(this.finish) this.finish(); this.readyState=4;
    if(op.onafterfinish) op.onafterfinish(this);
    this.dispose(); return;
  } if(op){

  var schedule = Math.sqrt((now-this.beginTime)/op.duration);
  if(op.trend!=true) schedule = Math.pow((this.endTime-now)/op.duration, 2);
  this.schedule = schedule;

  if(op.onbeforeupdate) op.onbeforeupdate(this);
  if(this.render)this.render(schedule);
  if(op.onafterupdate)op.onafterupdate(this);if(!this.terminative)

  this.timer=setTimeout(function(){me._process()},op.interval); else
  try{this.element.removeAttribute(this.attribName);}catch(ex){}}
  me.readyState=3; 
};
t.abort=function(){if(this.timer)clearTimeout(this.timer);};
t.end=function()
{
  if(this.readyState==4) return;
  if(this.timer) clearTimeout(this.timer);
  this.endTime = this.beginTime; this._process();
}
t.cancel=function()
{
  this.endTime = this.beginTime;
  if(this.timer)  clearTimeout(this.timer);
  if(this.restore) this.restore();
  this.dispose();
};
t.dispose=function()
{
  this.decontrol(); if(this.element)
  this.element.removeAttribute(this.attribName);
  delete this.timer;
  delete this.element;
  delete this.options;
  delete this.endTime;
  delete this.schedule;
  delete this.beginTime;
  delete this.attribName;
  delete this.terminative;
  if(System.ie) CollectGarbage();
};
MzEffect.formatColor=function(color)
{
  if(/^\#[\da-z]{6}$/i.test(color)) return color;
  else if(color.indexOf("rgb(")==0)
  {
    var cs=color.substring(4, color.length-1).split(",");
    for(var i=0, color="#"; i<cs.length; i++)
    {
      var s = parseInt(cs[i]).toString(16);
      color+= ("00"+ s).substr(s.length);
    }
    return color;
  }
  return "";
};


/*--====== core effect ======--*/
//op{interval, duration, trend}
MzEffect.Opacity=function(element, op)
{
  op = System.extend({interval: 20}, op||{});
  op.simple = (System.ie && MzBrowser.version<5.5)
    || (MzBrowser.opera  && MzBrowser.version<8.5);
  MzEffect.apply(this, [element, op]);
};
MzEffect.Opacity.Extends(MzEffect, "MzEffect.Opacity").initialize=function()
{
  var op=this.options, obj=this.element, me=this;
  this.render=function(schedule)
  {
    schedule = me.schedule || schedule;
    if(op.simple){me.endTime-=op.duration; return;}
    if(!System.ie)
    {
      obj.style.opacity = schedule;
      obj.style.MozOpacity = schedule;
      obj.style.KHTMLOpacity = schedule;
    }
    else obj.style.filter = "alpha(opacity:"+Math.round(schedule*100)+")";
  };
  if (System.ie)op.originalFilter = obj.style.filter; else
  {
    op.originalOpacity = obj.style.opacity;
    op.originalMozOpacity = obj.style.MozOpacity;
    op.originalKHTMLOpacity = obj.style.KHTMLOpacity;
  }
  this.restore=function()
  {
    if(System.ie) obj.style.filter = op.originalFilter; else
    {
      obj.style.opacity = op.originalOpacity;
      obj.style.MozOpacity = op.originalMozOpacity;
      obj.style.KHTMLOpacity = op.originalKHTMLOpacity;
    }
  };
};

//op{interval, duration, trend}
MzEffect.MoveBy=function(element, x, y, op)
{
  if("undefined"==typeof x || "undefined"==typeof y) return;
  this.offsetX=parseFloat(x);
  this.offsetY=parseFloat(y);
  op=System.extend({duration: 400}, op||{});
  MzEffect.apply(this, [element, op]);
};
MzEffect.MoveBy.Extends(MzEffect, "MzEffect.MoveBy").initialize=function()
{
  var me=this, obj=me.element, op=me.options;
  op.originalTop  = obj.style.top;
  op.originalLeft = obj.style.left;
  op.originalPosition = obj.style.position;
  me.originalY = parseFloat(obj.style.top  || '0');
  me.originalX = parseFloat(obj.style.left || '0');
  if(obj.style.position == "") obj.style.position = "relative";

  this.render=function(schedule)
  {
    schedule = me.schedule || schedule;
    obj.style.left = (me.offsetX * schedule + me.originalX) +"px";
    obj.style.top  = (me.offsetY * schedule + me.originalY) +"px";
  };
  this.setPosition=function(x, y)
  {
    obj.style.top  = y +"px";
    obj.style.left = x +"px";
  };
  this.restore=function()
  {
    obj.style.top  = op.originalTop;
    obj.style.left = op.originalLeft;
    obj.style.position = op.originalPosition;
  };
};

//op{interval, duration, trend}
MzEffect.MoveTo=function(element, x, y, op)
{
  if("undefined"==typeof x || "undefined"==typeof y) return;
  this.x=parseFloat(x);this.y=parseFloat(y);
  op=System.extend({duration: 300}, op||{});
  MzEffect.apply(this, [element, op]);
};
MzEffect.MoveTo.Extends(MzEffect, "MzEffect.MoveTo").initialize=function()
{
  var me=this, obj=me.element, op=me.options;
  op.originalTop  = obj.style.top;
  op.originalLeft = obj.style.left;
  op.originalPosition = obj.style.position;
  me.originalY = parseFloat(obj.style.top  || '0');
  me.originalX = parseFloat(obj.style.left || '0');
  me.offsetX = me.x - me.originalX;
  me.offsetY = me.y - me.originalY;
  if(obj.style.position == "") obj.style.position = "relative";

  this.render=function(schedule)
  {
    schedule = me.schedule || schedule;
    obj.style.left = (me.offsetX * schedule + me.originalX) +"px";
    obj.style.top  = (me.offsetY * schedule + me.originalY) +"px";
  };
  this.setPosition=function(x, y)
  {
    obj.style.top  = y +"px";
    obj.style.left = x +"px";
  };
  this.restore=function()
  {
    obj.style.top  = op.originalTop;
    obj.style.left = op.originalLeft;
    obj.style.position = op.originalPosition;
  };
};

//op{interval, duration, trend, color, beginColor, endColor, finalColor}
function n2h(s){s=parseInt(s).toString(16);return ("00"+ s).substr(s.length);}
MzEffect.Highlight=function(element, op){MzEffect.apply(this, arguments);};
MzEffect.Highlight.Extends(MzEffect, "MzEffect.Highlight").initialize=function()
{
  var op=this.options, obj=this.element, endColor="#FFFFFF";
  if(System.ie) var backColor = (obj.currentStyle||obj.style).backgroundColor;
  else var backColor = obj.getCssValue("background-color");
  op.originalBgColor = obj.style.backgroundColor;
  op.originalColor = obj.style.color;
  if(backColor) endColor = MzEffect.formatColor(backColor);
  op.beginColor = op.beginColor || "#FFFF00";
  op.endColor   = op.endColor || endColor;
  op.finalColor = op.finalColor || obj.style.backgroundColor;

  this.colors_base=[
    parseInt(op.beginColor.substring(1,3),16),
    parseInt(op.beginColor.substring(3,5),16),
    parseInt(op.beginColor.substr(5),16)];
  this.colors_var=[
    parseInt(op.endColor.substring(1,3),16)-this.colors_base[0],
    parseInt(op.endColor.substring(3,5),16)-this.colors_base[1],
    parseInt(op.endColor.substr(5),16)-this.colors_base[2]];

  this.finish=function()
  {
    if(op.color) obj.style.color = op.color;
    obj.style.backgroundColor = op.finalColor;
  };
  this.render=function(schedule)
  {
    schedule = this.schedule || schedule;
    var colors=[
      n2h(Math.round(this.colors_base[0]+(this.colors_var[0]*schedule))),
      n2h(Math.round(this.colors_base[1]+(this.colors_var[1]*schedule))),
      n2h(Math.round(this.colors_base[2]+(this.colors_var[2]*schedule)))];
    obj.style.backgroundColor = "#"+ colors.join("");
  };
  this.restore=function()
  {
    obj.style.color = op.originalColor;
    obj.style.backgroundColor = op.originalBgColor;
  };
  this.dispose=function()
  {
    MzEffect.prototype.dispose.call(this);
    delete this.colors_base;
    delete this.colors_var;
  };
};

MzEffect.Curtain=function(element, op)
{
  op = System.extend(
  {
    direction: "",
    scaleContent: false
  }, op||{});
  MzEffect.apply(this, [element, op]);
}
MzEffect.Curtain.Extends(MzEffect, "MzEffect.Mask").initialize=function()
{
  var me=this, op=me.options, obj=me.element;
  op.direction = op.direction.toLowerCase();

  op.originalWidth  = obj.style.width;
  op.originalHeight = obj.style.height;
  op.originalOverflow = obj.style.overflow;
  op.originalPosition = obj.style.position;
  op.originalVisibility = obj.style.visibility;
  op.originalMarginTop = (obj.currentStyle || obj.style).marginTop;

  op.originalOffsetWidth  = obj.offsetWidth;
  op.originalOffsetHeight = obj.offsetHeight;

  obj.style.overflow = "hidden";

  this.restore=function()
  {
    obj.style.width = op.originalWidth;
    obj.style.height = op.originalHeight;
    obj.style.overflow = op.originalOverflow;
    obj.style.position = op.originalPosition;
    obj.style.marginTop = op.originalMarginTop;
    obj.style.visibility = op.originalVisibility;
  };
  this.finish=function(){this.restore();};
  this.render=function(schedule){};
};

//op{interval, duration, trend, dirction}
MzEffect.Mask=function(element, op)
{
  op=System.extend({direction: "box"}, op||{});
  MzEffect.apply(this, [element, op]);
};
MzEffect.Mask.Extends(MzEffect, "MzEffect.Mask").initialize=function()
{
  var me=this, op=me.options, obj=me.element;
  if (!op.trend && obj.style.display=="none"){me.endTime-=op.duration; return;}
  op.direction = op.direction.toLowerCase();
  if(op.direction=="random")
  {
    var a=["box","center-up-down", "center-right-left", "top", "bottom",
      "left", "right", "top-right", "top-left", "right-top", "left-top"];
    var n=Math.floor(Math.random() * a.length);
    op.direction = (n>=a.length) ? "box" : a[n];
  }

  op.originalTop  = obj.style.top;
  op.originalLeft = obj.style.left;
  op.originalClip = obj.style.clip;
  op.originalWidth= obj.style.width;
  op.originalHeight= obj.style.height;
  if(System.ie) op.originalPosition = (obj.currentStyle || obj.style).position;
  else op.originalPosition = obj.getCssValue("position");
  if (op.trend){op.originalVisibility = obj.style.visibility;
  obj.style.visibility = "hidden"; MzElement.show(obj);}
  op.originalOffsetWidth  = obj.offsetWidth;
  op.originalOffsetHeight = obj.offsetHeight;
  if(System.ie || window.opera)
  {
    obj.style.width = op.originalOffsetWidth +"px";
    obj.style.height= op.originalOffsetHeight+"px";
  }
  else
  {
    obj.style.width = (op.originalOffsetWidth
      - parseInt(obj.style.borderLeftWidth||0)
      - parseInt(obj.style.borderRightWidth||0)) +"px";
    obj.style.height= (op.originalOffsetHeight
      - parseInt(obj.style.borderTopWidth||0)
      - parseInt(obj.style.borderBottomWidth||0)) +"px";
  }
  if (op.trend) obj.style.visibility = op.originalVisibility;

  //for opacity 20070114
  if (System.ie)op.originalFilter = obj.style.filter; else
  {
    op.originalOpacity = obj.style.opacity;
    op.originalMozOpacity = obj.style.MozOpacity;
    op.originalKHTMLOpacity = obj.style.KHTMLOpacity;
  }


  if(op.originalPosition!="absolute")
  {
    obj.style.position = "absolute";
    var input = document.createElement("INPUT");
    input.type="text";   input.style.border="none";
    input.readOnly=true; input.style.fontSize="1px";
    input.style.margin=input.style.padding=0;
    input.id = "MzEffectMaskStuffing"+ this.hashCode;
    input.style.backgroundColor = "transparent";//"red";
    var a=(obj.currentStyle||obj.style).marginLeft ||0;a=a=="auto"?0:parseInt(a);
    var b=(obj.currentStyle||obj.style).marginRight||0;b=b=="auto"?0:parseInt(b);
    input.style.width = (op.originalOffsetWidth  + a + b-(System.ie?2:0)) +"px";
    a=(obj.currentStyle||obj.style).marginTop   ||0;a=a=="auto"?0:parseInt(a);
    b=(obj.currentStyle||obj.style).marginBottom||0;b=b=="auto"?0:parseInt(b);
    input.style.height= (op.originalOffsetHeight + a + b-(System.ie?2:0)) +"px";
    obj.parentNode.insertBefore(input, obj.nextSibling);
  }

  this.restore=function()
  {
    obj.style.clip  = op.originalClip||"rect(auto, auto, auto, auto)";
    obj.style.top   = op.originalTop;
    obj.style.left  = op.originalLeft;
    obj.style.width = op.originalWidth;
    obj.style.height= op.originalHeight;
    obj.style.position = op.originalPosition;
    if(op.originalPosition!="absolute")
      MzElement.remove("MzEffectMaskStuffing"+ this.hashCode);
    if(System.ie) obj.style.filter = op.originalFilter; else
    {
      obj.style.opacity = op.originalOpacity;
      obj.style.MozOpacity = op.originalMozOpacity;
      obj.style.KHTMLOpacity = op.originalKHTMLOpacity;
    }
  };
  this.finish=function(){if(!op.trend)MzElement.hide(obj); me.restore();};
  this.render=function(schedule)
  {
    schedule = me.schedule || schedule;
    if(op.opacity)
    {
      if(!System.ie){obj.style.opacity = schedule;
        obj.style.MozOpacity = schedule; obj.style.KHTMLOpacity = schedule;}
      else obj.style.filter = "alpha(opacity:"+Math.round(schedule*100)+")";
    }
    switch(op.direction)
    {
      case "box":
        var halfW = Math.ceil(op.originalOffsetWidth /2);
        var halfH = Math.ceil(op.originalOffsetHeight/2);
        var right = Math.ceil(halfW * schedule);
        var bottom =Math.ceil(halfH * schedule);
        obj.style.clip = "rect("+ (halfH-bottom) +"px "+ (halfW+right) +"px "+
          (halfH+bottom) +"px "+ (halfW-right) +"px)";
        break;
      case "center-up-down":
      case "center-down-up":
      case "center-top-bottom":
      case "center-bottom-top":
        var halfH = Math.ceil(op.originalOffsetHeight/2);
        var bottom =Math.ceil(halfH * schedule);
        obj.style.clip = "rect("+ (halfH-bottom) +"px "+
          op.originalOffsetWidth +"px "+ (halfH+bottom) +"px 0)";
        break;
      case "center-right-left":
      case "center-left-right":
        var halfW = Math.ceil(op.originalOffsetWidth /2);
        var right = Math.ceil(halfW * schedule);
        obj.style.clip = "rect(0 "+ (halfW+right) +"px "+
          op.originalOffsetHeight +"px "+ (halfW-right) +"px)";
        break;
      case "top":
      case "top-bottom":
        var bottom =op.originalOffsetHeight * schedule +"px";
        obj.style.clip="rect(0 "+ op.originalOffsetWidth +"px "+ bottom +" 0)";
        break;
      case "bottom":
      case "bottom-top":
        var top = op.originalOffsetHeight * (1 - schedule) +"px";
        obj.style.clip = "rect("+ top +" "+ op.originalOffsetWidth +"px "+
          op.originalOffsetHeight +"px 0)";
        break;
      case "left":
      case "left-right":
        var right = op.originalOffsetWidth  * schedule +"px";
        obj.style.clip="rect(0 "+ right +" "+ op.originalOffsetHeight +"px 0)";
        break;
      case "right":
      case "right-left":
        var left =op.originalOffsetWidth  * (1 - schedule) +"px";
        obj.style.clip = "rect(0 "+ op.originalOffsetWidth +"px "+
          op.originalOffsetHeight +"px "+ left +")";
        break;
      case "top-right":
      case "left-bottom":
        var right = op.originalOffsetWidth  * schedule +"px";
        var bottom =op.originalOffsetHeight * schedule +"px";
        obj.style.clip = "rect(0 "+ right +" "+ bottom +" 0)";
        break;
      case "top-left":
      case "right-bottom":
        var bottom = op.originalOffsetHeight * schedule +"px";
        var left =op.originalOffsetWidth  * (1 - schedule) +"px";
        obj.style.clip = "rect(0 "+ op.originalOffsetWidth +"px "+
          bottom +" "+ left +")";
        break;
      case "right-top":
      case "bottom-left":
        var top = op.originalOffsetHeight * (1 - schedule) +"px";
        var left =op.originalOffsetWidth  * (1 - schedule) +"px";
        obj.style.clip = "rect("+ top +" "+ op.originalOffsetWidth +"px "+
          op.originalOffsetHeight +"px "+ left +")";
        break;
      case "left-top":
      case "bottom-right":
        var top = op.originalOffsetHeight * (1 - schedule) +"px";
        var right=op.originalOffsetWidth  * schedule +"px";
        obj.style.clip = "rect("+ top  +" "+ right +" "+
          op.originalOffsetHeight +"px 0)";
        break;
    }
  };
};


//op{interval, duration, trend}
MzEffect.Combo=function(effects, element, op)
{
  this.effects=effects||[];
  if(this.effects.length==0) return;
  MzEffect.apply(this,[element,op]);
};
t=MzEffect.Combo.Extends(MzEffect, "MzEffect.Combo");
t.render=function(schedule)
{
  schedule = this.schedule || schedule;
  for(var i=0; i<this.effects.length; i++)
  {
    var e = this.effects[i];
    if (e.timer) clearTimeout(e.timer);
    e.render(schedule);
  }
};
t.finish=function()
{
  for(var i=0; i<this.effects.length; i++)
  {
    if(this.effects[i].finish) this.effects[i].finish();
    this.effects[i].dispose();
  }
};
t.dispose=function()
{
  for(var i=0; i<this.effects.length; i++) this.effects[i].dispose();
  MzEffect.prototype.dispose.call(this);
  delete this.effects;
};





/*--====== meizz effects ======--*/

MzEffect.Shake=function(element){MzEffect.call(this, element)};
MzEffect.Shake.Extends(MzEffect, "MzEffect.Shake").initialize=function()
{
  var me=this, obj=me.element, os=obj.style;
  var top =os.top, left=os.left, position=os.position;
  this.restore=function(){os.top=top; os.left=left; os.position=position;}

  this._process=function()
  {
    var a=[0, 0];
    if(me._scheduledIndex>=me._scheduledArray.length)
    {
      this._process=function()
      {
        me.restore();
        me.dispose();
        delete me._scheduledIndex;
        delete me._scheduledArray;
      };
    }
    else
    {
      a=me._scheduledArray[me._scheduledIndex];
      me._scheduledIndex++;
    }
    MzEffect.moveBy(obj, a[0], a[1],{
      duration: 40,
      interval: 20,
      onafterfinish: function(e){me._process();}});
  };
};
MzEffect.Shake.prototype._scheduledIndex=0;
MzEffect.Shake.prototype._scheduledArray=
[
   [20, -5]
  ,[-40,-5]
  ,[40, 10]
  ,[-40, 5]
  ,[40, -9]
  ,[-35, 8]
  ,[35, -7]
  ,[-30, 6]
  ,[20, -5]
  ,[-15, 4]
  ,[10, -3]
  ,[-8,  2]
  ,[6,  -1]
  ,[-5,  0]
  ,[3,   0]
  ,[-1,  0]
];


MzEffect.Appear=function(element, op)
{
  var e=MzElement.check(element);
  if(!e) return; if(System.ie 
    &&!e.style.width
    &&!e.style.height
    &&e.style.position!="absolute"
    &&e.currentStyle
    &&e.currentStyle.position!="absolute"
    &&e.currentStyle.width=="auto"
    &&e.currentStyle.height=="auto")
  { MzElement.show(e); return;}
  op=System.extend(
  {
    onbeforestart: function(e){MzElement.show(e.element);e.render(0.01);},
    onafterfinish: function(e){e.restore();}
  }, op||{});
  return MzEffect.opacity(element, op);
};

MzEffect.Fade=function(element, op)
{
  var e=MzElement.check(element);
  if(!e) return; if(System.ie 
    &&!e.style.width
    &&!e.style.height
    &&e.style.position!="absolute"
    &&e.currentStyle
    &&e.currentStyle.position!="absolute"
    &&e.currentStyle.width=="auto"
    &&e.currentStyle.height=="auto")
  { MzElement.hide(e); return;}
  op=System.extend(
  {
    trend: "weaken",
    onafterfinish: function(e){MzElement.hide(e.element); e.restore();}
  }, op||{});
  return MzEffect.opacity(element, op);
};


//op{loop, interval, dynamic}
MzEffect.Pulsate=function(element, op)
{
  this.element=MzElement.check(element); if(!this.element) return;
  op=this.options=System.extend({loop:5,interval:360,dynamic:true},op||{});
  if(!op.dynamic || op.loop<0 || op.loop>5) op.interval += 160;

  var a=this.attribName = "att_"+ this._className.replace(/\W/g, "_");
  if((t=this.element.getAttribute(a)) && (t=Instance(t))){
  System.extend(t.options, this.options); t.times=0; return t;}

  System.call(this);
  this.element.setAttribute(this.attribName, this.hashCode, 0);
  this.timer=null; this.initialize();
  if(op.onbeforestart) op.onbeforestart(this); this.render();
};
t=MzEffect.Pulsate.Extends(System, "MzEffect.Pulsate");

t.times=0;
t.status=true;
t.initialize=function()
{
  var me=this, op=me.options, obj=me.element;
  op.visibility=obj.style.visibility;

  this.render=function()
  {
    if(me.terminative) return me.stop();
    if(op.loop>-1 && me.times>=op.loop) return me.stop();
    if(me.status)
    {
      if(!op.dynamic || op.loop<0 || op.loop>5)
      {
        obj.style.visibility="hidden";
        me.timer = setTimeout(me.render, op.interval);
      }
      else
      {
        MzEffect.fade(obj, {
          interval: 40,
          duration: 160,
          onafterfinish: function(e)
          {
            obj.style.visibility="hidden";
            e.restore();
            me.timer = setTimeout(me.render, op.interval);
          }
        });
      }
    }
    else
    {
      if(!op.dynamic || op.loop<0 || op.loop>5)
      {
        obj.style.visibility="";
        me.timer = setTimeout(me.render, op.interval);
      }
      else
      {
        MzEffect.appear(obj,{
          interval: 40,
          duration: 160,
          onbeforestart: function(e){e.element.style.visibility=""; e.render(0.01);},
          onafterfinish: function(e){e.restore(); me.timer = setTimeout(me.render, op.interval);}
        });
      }
    }
    me.status = !me.status; if(me.status) me.times++;
  };

  this.stop=function()
  {
    obj.style.visibility=op.visibility;
    if(me.finish) me.finish();
    if(op.onafterfinish) op.onafterfinish(me);
    clearTimeout(me.timer);
    me.dispose();
  }
  this.dispose=function()
  {
    me.decontrol(); if(me.element)
    me.element.removeAttribute(me.attribName);
    delete me.state;
    delete me.timer;
    delete me.times;
    delete me.element;
    delete me.options;
  };
};
MzEffect.Pulsate.stop=function(element)
{
  if(element=MzElement.check(element))
  {
    var t = "att_"+ "MzEffect.Pulsate".replace(/\W/g, "_");
    if(t=element.getAttribute(t))if(t=Instance(t))t.terminative=true;
  }
}


//op{interval, duration}
MzEffect.BlindDown=function(element, op){MzEffect.apply(this, arguments);};
MzEffect.BlindDown.Extends(MzEffect, "MzEffect.Mask").initialize=function()
{
  var op=this.options, obj=this.element;
  op.height = obj.style.height;
  op.overflow = obj.style.overflow;
  op.visibility = obj.style.visibility;
  obj.style.visibility = "hidden";
  MzElement.show(obj);
  op.offsetHeight = obj.offsetHeight; if(!System.ie){
  var t; if(t=obj.style.paddingTop) t=parseFloat(t);
  if("number"==typeof t) op.offsetHeight -= t;
  var b; if(b=obj.style.paddingBottom) b=parseFloat(b);
  if("number"==typeof b) op.offsetHeight -= b;}
  obj.style.height = "1px"; obj.style.overflow = "hidden";
  setTimeout(function(){obj.style.visibility = op.visibility;}, 10);

  this.finish=this.restore=function()
  {
    obj.style.overflow = op.overflow;
    obj.style.height = op.height;
  };
  this.render=function(schedule)
  {
    schedule = this.schedule || schedule;
    obj.style.height = parseInt(schedule * op.offsetHeight) +"px";
  };
};

//op{interval, duration}
MzEffect.BlindUp=function(element, op)
{
  op = System.extend({trend: "weaken"}, op||{});
  MzEffect.apply(this, [element, op]);
};
MzEffect.BlindUp.Extends(MzEffect, "MzEffect.Mask").initialize=function()
{
  var op=this.options, obj=this.element;
  op.height = obj.style.height;
  op.overflow = obj.style.overflow;
  op.offsetHeight = obj.offsetHeight; if(!System.ie){
  var t; if(t=obj.style.paddingTop) t=parseFloat(t);
  if("number"==typeof t) op.offsetHeight -= t;
  var b; if(b=obj.style.paddingBottom) b=parseFloat(b);
  if("number"==typeof b) op.offsetHeight -= b;}
  obj.style.overflow = "hidden";

  this.finish=function()
  {
    MzElement.hide(obj);
    obj.style.overflow = op.overflow;
    obj.style.height = op.height;
  };
  this.restore=function()
  {
    obj.style.overflow = op.overflow;
    obj.style.height = op.height;
  };
  this.render=function(schedule)
  {
    schedule = this.schedule || schedule;
    if(schedule<=0.05) MzElement.hide(obj);
    obj.style.height = parseInt(schedule * op.offsetHeight) +"px";
  };
};

//op{interval, duration}
MzEffect.SlideUp=function(element, op)
{
  return MzEffect.collapse(element, System.extend(
  {
    onafterupdate: function(e){e.element.scrollTop=e.options.offsetHeight;}
  }, op||{}))
};

//op{interval, duration}
MzEffect.SlideDown=function(element, op)
{
  return MzEffect.expand(element, System.extend(
  {
    onafterupdate: function(e){e.element.scrollTop=e.options.offsetHeight;}
  }, op||{}))
};

//op{interval, duration}
MzEffect.Fall=function(){MzEffect.Curtain.apply(this, arguments);};
MzEffect.Fall.Extends(MzEffect.Curtain).initialize=function()
{
  MzEffect.Curtain.prototype.initialize.call(this);
  var me=this, op=me.options, obj=me.element;

  this.finish=function(){this.hide(9);};
  this.hide=function(n)
  {
    if(n<0){MzElement.hide(obj); me.restore(); return;}
    obj.style.height = "1px"; obj.style.visibility = "hidden";
    obj.style.marginTop=(n/10)*op.originalOffsetHeight + parseFloat(op.originalMarginTop||0) +"px";
    this.timer=setTimeout(function(){me.hide(--n);}, 10);
  }
  this.render=function(schedule)
  {
    schedule = Math.ceil(op.originalOffsetHeight * (this.schedule || schedule));
    obj.style.marginTop = (schedule + parseFloat(op.originalMarginTop||0)) +"px";
    obj.style.height= (op.originalOffsetHeight - schedule) +"px";
  };
};

//op{interval, duration}
MzEffect.Rise=function(){MzEffect.Curtain.apply(this, arguments);};
MzEffect.Rise.Extends(MzEffect.Curtain).initialize=function()
{
  MzEffect.Curtain.prototype.initialize.call(this);
  var me=this, op=me.options, obj=me.element;

  obj.style.height="1px"; MzElement.show(obj);
  op.originalOffsetHeight = obj.offsetHeight;
  obj.style.marginTop = op.originalOffsetHeight + parseFloat(op.originalMarginTop||0);

  this.render=function(schedule)
  {
    schedule = Math.ceil(op.originalOffsetHeight * (this.schedule || schedule));
    obj.style.height = schedule +"px";
    obj.style.marginTop = (op.originalOffsetHeight - schedule + parseFloat(op.originalMarginTop||0)) +"px";
  };
};





//[static method]
MzEffect.opacity  =function(element, op){return new MzEffect.Opacity(element, op);};
MzEffect.moveBy   =function(element, op){return new MzEffect.MoveBy(element, op);};
MzEffect.moveTo   =function(element, op){return new MzEffect.MoveTo(element, op);};
MzEffect.highlight=function(element, op){return new MzEffect.Highlight(element, op);};
MzEffect.mask     =function(element, op){return new MzEffect.Mask(element, op);};

MzEffect.combo    =function(element, op){};
MzEffect.shake    =function(element, op){return new MzEffect.Shake(element, op);};
MzEffect.appear   =function(element, op){return new MzEffect.Appear(element, op);};
MzEffect.fade     =function(element, op){return new MzEffect.Fade(element, op);};
MzEffect.pulsate  =function(element, op){return new MzEffect.Pulsate(element, op);};
MzEffect.expand   =function(element, op){return new MzEffect.BlindDown(element, op);};
MzEffect.collapse =function(element, op){return new MzEffect.BlindUp(element, op);};
MzEffect.remove   =function(element, op){return new MzEffect.Fade(element, System.extend(op||{}, {onafterfinish: function(){MzElement.remove(element)}}));};
