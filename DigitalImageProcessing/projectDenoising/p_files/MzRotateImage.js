/*---------------------------------------------------------------------------*\
|  Subject:    Rotate AD
|  NameSpace:  System.Web.UI.WebControls.MzRotateImage
|  Author:     meizz
|  Created:    2006-11-11
|  Version:    2006-12-06
|-----------------------------------
|  MSN: huangfr@msn.com QQ:112889082 Copyright (c) meizz
|  http://www.meizz.com/jsframework/ MIT-style license
|  The above copyright notice and this permission notice shall be
|  included in all copies or substantial portions of the Software
\*---------------------------------------------------------------------------*/
//Using("System.Data.MzDataProvider");
//Using("System.Web.Forms.MzBehavior");

//node{url, target, summary, img, alt}
function MzRotateImage()
{
  MzDataProvider.call(this); this.stateChangeHandle(1);

  this.width = 280;
  this.height= 187;
  this.timer = null;
  this.interval = 3000;
  this.duration = 2000;
  this.activeIndex = 1;
  this.currentIndex = 0;
  this.floatControlBar = false;
  this.useFilter = System.ie && MzBrowser.version>=5.5;
}
MzRotateImage.Extends(MzDataProvider, "MzRotateImage");
System.loadCssFile(System.resourcePath +"/MzRotateImage.css", "MzRotateImage_CSS");

MzRotateImage.prototype.render=function()
{
  this.dataInit();  this.images=new Array();
  var d = this.nodes = this.rootNode.childNodes;

  for(var i=0; i<d.length; i++)
  {
    this.images[i] = new Image();
    this.images[i].src = d[i].get("img");
  }

  var id=this.id="MzRotateImage_"+this.hashCode,s=[];
  var width  = this.width  = parseInt(this.width);
  var height = this.height = parseInt(this.height);

  s.push("<div id='"+id+"' style='width:"+width+"px;' class='MzRotateImage'>");
  s.push("<div id='"+id+"_ImageBox' class='MzRotateImage_ImageBox' style='height:"+ height +"px'>");
  if(this.useFilter) { if(d.length>0) //filter: revealTrans
  {
    var alt = d[0].get("alt"), src = this.images[0].src;
    s.push("<a href='#'><img alt='"+ alt +"' src='"+src+"' ");if(d.length>1)
    s.push("style='filter:revealTrans(duration="+(this.duration/1000)+")'");
    s.push(" id='"+ id +"_img' style='border: none' /></a>");}
  }
  else { for(i=0; i<d.length; i++) //new MzBehavior.Rotate()
  {
    s.push("<div id='"+id+"_item_"+i+"' style='width: "+width+"px;");
    if (i>0) s.push(" display: none;");
    s.push(" height: "+ height +"px; overflow: hidden;'>");
    s.push("<a href='"+ (d[i].get("url") || "#")+"'");
    s.push(" target='"+ (d[i].get("target") || "_self") +"'>");
    s.push("<img alt='"+(d[i].get("alt") || "") +"'");
    s.push(" src='"+ this.images[i].src +"' /></a></div>");}
  }
  s.push("</div><div style='width: "+ width +"px; ");
  s.push((this.floatControlBar?"margin-top: -16px":"") +"' ");
  s.push(" id='"+id+"_ControlBar' class='MzRotateImage_ControlBar'>");
  for(i=0;i<d.length;i++)s.push("<input type='button' value='"+(i+1)+"'/>");
  s.push("</div>"); s.push("</div>"); s = s.join("");
  this.stateChangeHandle(2); this._onload();
  return s;
};

MzRotateImage.prototype.stateChangeHandle=function(n)
{
  this.readyState = n||0;
  this.dispatchEvent(new System.Event("onreadystatechange"));
};
MzRotateImage.prototype._onload=function()
{
  var me=this;
  if(MzElement.check(this.id))
  {
    this.stateChangeHandle(4);
    
    if(this.useFilter) this.timer=
      setTimeout(function(){me.filter();}, me.interval+me.duration);
    else
    {
      this._rotate = new MzBehavior.Rotate(me.id +"_ImageBox",
        {interval:me.interval,duration:me.duration});
      this._rotate.addEventListeners("onchange", function(e)
      {
        me.activeIndex  = e.target.activeIndex;
        me.currentIndex = e.target.currentIndex;
        e= new System.Event("onchange"); e.target=me;
        me.dispatchEvent(e);
      });
    }
    
    var A = MzElement.check(this.id+"_ControlBar").getElementsByTagName("INPUT");
    A[this.currentIndex].className = "active";

    this.addEventListeners("onchange", function(e)
    {
      for(var i=0; i<A.length; i++) A[i].className="";
      A[e.target.activeIndex].className = "active";
    });

    for(var i=0; i<A.length; i++)
    {
      var f=new Function("Instance('"+ this.hashCode +"').focus("+ i +")");
      A[i].onmouseover = f; A[i].onclick = f;
    }
  }
  else setTimeout(function(){me._onload();}, 10);
};

MzRotateImage.prototype.focus=function(n)
{
  clearTimeout(this.timer);
  if(this.useFilter){this.activeIndex=n; this.filter();}
  else if(this._rotate) this._rotate.focus(n);
};
MzRotateImage.prototype.filter=function()
{
  var me = this;
  if(me.dispatchEvent(new System.Event("onchange")))
  {
    var img; if(img=MzElement.check(me.id +"_img")){
    img.filters.revealTrans.Transition=23;
    img.filters.revealTrans.apply();
    var a = img.parentNode;
    var N=me.activeIndex;
    this.currentIndex = N;
    this.activeIndex = N+1>=me.nodes.length ? 0 : N+1;

    a.href = (me.nodes[N].get("url") || "#");
    a.target = (me.nodes[N].get("target") || "_self");
    img.src=me.images[N].src;
    img.filters.revealTrans.play();}
  }
  this.timer=setTimeout(function(){me.filter();}, me.interval+me.duration);
};
