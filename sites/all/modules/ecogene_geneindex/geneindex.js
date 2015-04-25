/**
 * 
 */

//NAVIGATION
sfHover = function() {
	chromeAddMouseEvents("gene-menu-nav-contain");
};

if (window.attachEvent) {
	window.attachEvent("onload", sfHover);
}

function chromeAddMouseEvents(divID) {
 var containerDiv = document.getElementById(divID);
 if (containerDiv) {
	 var sfEls = containerDiv.getElementsByTagName('li');
	 for (var i=sfEls.length; i--; ) {
		 	sfEls[i].onmouseover = chromeMouseOver;
		 	sfEls[i].onmouseout = chromeMouseOut;
	 }
 }
}

function chromeMouseOver() {
	this.className+=' sfhover';
}
function chromeMouseOut() {
	this.className=this.className.replace(new RegExp(' sfhover\\b'), '');
} 