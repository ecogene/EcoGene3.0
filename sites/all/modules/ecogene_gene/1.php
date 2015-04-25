<?php


$dbName   = 'ecogene';

$dbTable  = 't_gene_multi_address';


//$eg_id = "EG40002";

// Get connection to MySQL

//mysql_connect($hostname, $sqluser, $sqlpass) or die('Unable to connect to database'); by dfan 5/18/07

@mysql_select_db($dbName) or die('Unable to select database');





# Get menu

# phpddm_menu(TITLE, VTITLE, WIDTH, URL, TARGET)

$result = mysql_query("SELECT * FROM ".$dbTable." WHERE eg_id= '".$eg_id."' ORDER by left_end ASC");





?>





<style type="text/css">



#dropmenudiv{
position:absolute;
border:1px solid gray;
border-bottom-width: 0;
font:normal 12px Verdana;
line-height:18px;
z-index:100;
}

#dropmenudiv a{
width: 100%;
display: block;
text-indent: 3px;
border-bottom: 1px solid gray;
padding: 1px 0;
text-decoration: none;
font-weight: ;
color: black;
}



#dropmenudiv a:hover{ /*hover background color*/
background-color: white
}



</style>





<script type="text/javascript">



/***********************************************

* AnyLink Drop Down Menu- ? Dynamic Drive (www.dynamicdrive.com)

* This notice MUST stay intact for legal use

* Visit http://www.dynamicdrive.com/ for full source code

***********************************************/



//Contents for menu 1

var menu1 = new Array()

<?php 

$number = mysql_numrows($result);

echo "menu1[0]= '<a>Multiple Addresses</a>'\n";

for ($i=0; $i<$number; $i++) {

	$add_left = mysql_result($result, $i, 'left_end');

	$add_right = mysql_result($result, $i, 'right_end');

	$t = $i+1;

    echo "menu1[".$t."]= '<a href=\"?q=gene/$eg_id&add_left_end=".$add_left."\">[$add_left $add_right]</a>'\n";

}

 ?>

//Contents for menu 2, and so on



var menuwidth='165px' //default menu width

var menubgcolor='lightblue' //menu bgcolor

var disappeardelay=250 //menu disappear speed onMouseout (in miliseconds)

var hidemenu_onclick="yes" //hide menu when user clicks within menu?



/////No further editting needed



var ie4=document.all

var ns6=document.getElementById&&!document.all



if (ie4||ns6)

document.write('<div id="dropmenudiv" style="visibility:hidden;width:'+menuwidth+';background-color:'+menubgcolor+'" onMouseover="clearhidemenu()" onMouseout="dynamichide(event)"></div>')



function getposOffset(what, offsettype){

var totaloffset=(offsettype=="left")? what.offsetLeft : what.offsetTop;

var parentEl=what.offsetParent;

while (parentEl!=null){

totaloffset=(offsettype=="left")? totaloffset+parentEl.offsetLeft : totaloffset+parentEl.offsetTop;

parentEl=parentEl.offsetParent;

}

return totaloffset;

}





function showhide(obj, e, visible, hidden, menuwidth){

if (ie4||ns6)

dropmenuobj.style.left=dropmenuobj.style.top=-500

if (menuwidth!=""){

dropmenuobj.widthobj=dropmenuobj.style

dropmenuobj.widthobj.width=menuwidth

}

if (e.type=="click" && obj.visibility==hidden || e.type=="mouseover")

obj.visibility=visible

else if (e.type=="click")

obj.visibility=hidden

}



function iecompattest(){

return (document.compatMode && document.compatMode!="BackCompat")? document.documentElement : document.body

}



function clearbrowseredge(obj, whichedge){

var edgeoffset=0

if (whichedge=="rightedge"){

var windowedge=ie4 && !window.opera? iecompattest().scrollLeft+iecompattest().clientWidth-15 : window.pageXOffset+window.innerWidth-15

dropmenuobj.contentmeasure=dropmenuobj.offsetWidth

if (windowedge-dropmenuobj.x < dropmenuobj.contentmeasure)

edgeoffset=dropmenuobj.contentmeasure-obj.offsetWidth

}

else{

var topedge=ie4 && !window.opera? iecompattest().scrollTop : window.pageYOffset

var windowedge=ie4 && !window.opera? iecompattest().scrollTop+iecompattest().clientHeight-15 : window.pageYOffset+window.innerHeight-18

dropmenuobj.contentmeasure=dropmenuobj.offsetHeight

if (windowedge-dropmenuobj.y < dropmenuobj.contentmeasure){ //move up?

edgeoffset=dropmenuobj.contentmeasure+obj.offsetHeight

if ((dropmenuobj.y-topedge)<dropmenuobj.contentmeasure) //up no good either?

edgeoffset=dropmenuobj.y+obj.offsetHeight-topedge

}

}

return edgeoffset

}



function populatemenu(what){

if (ie4||ns6)

dropmenuobj.innerHTML=what.join("")

}





function dropdownmenu(obj, e, menucontents, menuwidth){

if (window.event) event.cancelBubble=true

else if (e.stopPropagation) e.stopPropagation()

clearhidemenu()

dropmenuobj=document.getElementById? document.getElementById("dropmenudiv") : dropmenudiv

populatemenu(menucontents)



if (ie4||ns6){

showhide(dropmenuobj.style, e, "visible", "hidden", menuwidth)

dropmenuobj.x=getposOffset(obj, "left")

dropmenuobj.y=getposOffset(obj, "top")

dropmenuobj.style.left=dropmenuobj.x-clearbrowseredge(obj, "rightedge")+"px"

dropmenuobj.style.top=dropmenuobj.y-clearbrowseredge(obj, "bottomedge")+obj.offsetHeight+"px"

}



return clickreturnvalue()

}



function clickreturnvalue(){

if (ie4||ns6) return false

else return true

}



function contains_ns6(a, b) {

while (b.parentNode)

if ((b = b.parentNode) == a)

return true;

return false;

}



function dynamichide(e){

if (ie4&&!dropmenuobj.contains(e.toElement))

delayhidemenu()

else if (ns6&&e.currentTarget!= e.relatedTarget&& !contains_ns6(e.currentTarget, e.relatedTarget))

delayhidemenu()

}



function hidemenu(e){

if (typeof dropmenuobj!="undefined"){

if (ie4||ns6)

dropmenuobj.style.visibility="hidden"

}

}



function delayhidemenu(){

if (ie4||ns6)

delayhide=setTimeout("hidemenu()",disappeardelay)

}



function clearhidemenu(){

if (typeof delayhide!="undefined")

clearTimeout(delayhide)

}



if (hidemenu_onclick=="yes")

document.onclick=hidemenu



</script>










