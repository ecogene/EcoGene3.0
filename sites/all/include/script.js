jQuery(document).ready(function(){
	/* This code is executed after the DOM has been completely loaded */
	
	var totWidth=0;
	var positions = new Array();
	
	jQuery('#gallery_slides .slide').each(function(i){
		
		/* Traverse through all the slides and store their accumulative widths in totWidth */
		
		positions[i]= totWidth;
		totWidth += jQuery(this).width();
		
		/* The positions array contains each slide's commulutative offset from the left part of the container */
		
		if(!jQuery(this).width())
		{
			alert("Please, fill in width & height for all your images!");
			return false;
		}
		
	});
	
	jQuery('#gallery_slides').width(totWidth);

	/* Change the cotnainer div's width to the exact width of all the slides combined */

	jQuery('#gallery_menu ul li a').click(function(e,keepScroll){

			/* On a thumbnail click */

		jQuery('#gallery_menu li.menuItem').removeClass('act').addClass('inact');
		jQuery(this).parent().addClass('act');
			
			var pos = jQuery(this).parent().prevAll('.menuItem').length;
			
			jQuery('#gallery_slides').stop().animate({marginLeft:-positions[pos]+'px'},450);
			/* Start the sliding animation */
			
			e.preventDefault();
			/* Prevent the default action of the link */
			
			
			// Stopping the auto-advance if an icon has been clicked:
			if(!keepScroll) clearInterval(itvl);
	});
	
	jQuery('#gallery_menu ul li.menuItem:first').addClass('act').siblings().addClass('inact');
	/* On page load, mark the first thumbnail as active */
	
	
	
	/*****
	 *
	 *	Enabling auto-advance.
	 *
	 ****/
	 
	var current=1;
	function autoAdvance()
	{
		if(current==-1) return false;
		
		jQuery('#gallery_menu ul li a').eq(current%jQuery('#gallery_menu ul li a').length).trigger('click',[true]);	// [true] will be passed as the keepScroll parameter of the click function on line 28
		current++;
	}

	// The number of seconds that the slider will auto-advance in:
	
	var changeEvery = 10;

	var itvl = setInterval(function(){autoAdvance()},changeEvery*1000);

	/* End of customizations */
});