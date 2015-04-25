
function toggleVisCol(elem){
	var name = elem.name;
	
	
	if (name.indexOf('[') != -1)	classname = 'tcol_'+name.substring(name.indexOf('[')+2, name.indexOf(']'));
	else classname = 'tcol_'+ name.substring(1);
	
	// However, IE5 at least does not render table cells correctly
	// using the style 'table-cell', but does when the style 'block'
	// is used, so handle this
	var showMode = 'table-cell';
	if (document.all) showMode='block';
	// Once the cells and checkbox object has been retrieved
	// the show hide choice is simply whether the checkbox is
	// checked or clear
	mode = elem.checked ? showMode : 'none';
	
	var list = document.getElementsByClassName(classname);

		for (var i = 0; i < list.length; i++) {
			list[i].style.display = mode;
			
		    // list[i] is a node with the desired class name
	}
	
		
}
function SetAllCheckBoxes(FormName, FieldName, CheckValue)
{
	if(!document.forms[FormName])
		return;
	form = document.forms[FormName];
	
	for(i=0;i<form.elements.length;i++){
		
		if((form.elements[i].type=="checkbox") &&  (form.elements[i].name.indexOf(FieldName) !=-1))
		{
			form.elements[i].checked = CheckValue;
			toggleVisCol(form.elements[i]);
		}
	}
	
}

function submit_sequence_download(aform)
{


	var reversed, from, to, genome

	from = aform.from.value
 	to = aform.to.value
 	genome = aform.genome_name.value

 	if (aform.radio[0].checked)
 	{
  		reversed = 0
 	}
 	else
 	{
  		reversed = 1
 //   	alert("reversed= "+reversed+" from= "+from +" to= "+to) 
 	}
	var alertMessage=""
 //check from field
	if(from==""|| isNaN(from))
		alertMessage = alertMessage + "Please  input an interger in \"From\" field\n"
 //check to field	
	if(to=="" || isNaN(to))
		alertMessage = alertMessage + "Please input an interger in \"To\" field \n"
	if(parseInt(to)<parseInt(from))
		alertMessage = alertMessage + "The value of To must be larger than the value of From!\n"	
	if(genome=="")
		alertMessage = alertMessage + "Please select a genome sequence!\n"		

	if(alertMessage=="")	
	{
		aform.action='?q=ecodownload/sequence'
//		aform.submit
	}
	else
		alert(alertMessage)   

}

function change_sequence()
{

//	var aform = document.getElementById('sequence_form');
	 var aform = document.forms['sequence_form'];
	 if (aform.genome_name.value=='ECOLI_3')
	 {
	 		aform.from.value='1';
	 		aform.to.value='4641652';
	 }else if (aform.genome_name.value=='ECOLI_2')
 	{
 		aform.from.value='1';
 		aform.to.value='4639675';
 	}else if(aform.genome_name.value=='ECOLI_1')
 	{
 		aform.from.value='1';
 		aform.to.value='4639211';
 	}else
 	{
 		aform.from.value='';
 		aform.to.value='';
 	}
}
function change_download_primer_form()
{
		var form=document.getElementById("download-primer-form");
		var sfEls = form.getElementsByTagName('div');
//		$('edit-del-addon-type.removeClassName').('form-disabled');
//		 for (var i=sfEls.length; i--; ) {
//			 if(sfEls[i].className.indexOf('form-disabled') !=-1)
//				{
//				 alert(sfEls[i].ClassName);
////				 sfEls[i].removeClassName('form-disabled');
////				 alert(sfEls[i].class);
////				 sfEls[i].ClassName = sfEls[i].className.replace(new RegExp(' form-disabled\\b'), ''); 
////				 alert(sfEls[i].ClassName);
//				}
//		 }
		for(i=0;i<form.elements.length;i++){					
				form.elements[i].disabled = false;			
			
		}
		
		form.vstart_co.disabled = false;		
		form.vstart_co.checked = true;
		
		form.vstop_co.checked = true;
		form.vstop_co.disabled = false;
		
//		form.del_offset_type.disabled = false; 		
		form.del_start_offset.disabled = false; 
		form.del_stop_offset.disabled = false; 		
		form.del_addon_type[0].disabled = false; 	
		form.del_addon_type[1].disabled = false; 	
		form.del_addon_type[2].disabled = false; 	
		form.del_addon_type[3].disabled = false; 
		form.del_start_add_ons.disabled = false; 
		form.del_stop_add_ons.disabled = false; 	
		

		form.clo_offset_type_start[0].disabled = false; 
		form.clo_offset_type_start[1].disabled = false; 	
		form.clo_offset_type_stop[0].disabled = false; 
		form.clo_offset_type_stop[1].disabled = false; 	
		
		form.clo_start_restrict.disabled = false;
		form.clo_stop_restrict.disabled = false;
		
//		alert((form.clo_offset_type));
//		if(form.clo_offset_type.length>0) 		
		if(typeof(form.clo_offset_type) != "undefined"){ form.clo_offset_type[0].disabled = false;	  form.clo_offset_type[1].disabled = false;	}
		form.clo_start_offset.disabled = false; 
		form.clo_stop_offset.disabled = false; 		
		form.clo_addon_type[0].disabled = false; 	
		form.clo_addon_type[1].disabled = false; 	
		form.clo_start_add_ons.disabled = false; 
		form.clo_stop_add_ons.disabled = false; 	
		

			
	
 	if(form.gene_type[1].checked)
 	{
 		form.vstart_co.checked = false; 
		form.vstop_co.checked = false;
		form.vstart_co.disabled = true; 
		form.vstop_co.disabled = true;
 	}
 	if(form.clon_del[0].checked) // cloning is chosen
 	{
 				
//		form.del_offset_type.disabled = true; 
		
		form.del_start_offset.disabled = true; 
		form.del_stop_offset.disabled = true; 
		
		form.del_addon_type[0].disabled = true; 	
		form.del_addon_type[1].disabled = true; 	
		form.del_addon_type[2].disabled = true; 	
		form.del_addon_type[3].disabled = false; 
		

		form.del_start_add_ons.disabled = true; 
		form.del_stop_add_ons.disabled = true; 

 	}else
 	{
 		form.clo_start_restrict.disabled = true;
		form.clo_stop_restrict.disabled = true;
 				
		form.clo_offset_type_start[0].disabled = true; 
		form.clo_offset_type_start[1].disabled = true; 
		form.clo_offset_type_stop[0].disabled = true; 
		form.clo_offset_type_stop[1].disabled = true; 	
//		if(form.clo_offset_type.length>0)       
		if(typeof(form.clo_offset_type) != "undefined"){ form.clo_offset_type[0].disabled = true;	  form.clo_offset_type[1].disabled = true;	}	
		form.clo_start_offset.disabled = true; 
		form.clo_stop_offset.disabled = true; 		
		form.clo_addon_type[0].disabled = true; 	
		form.clo_addon_type[1].disabled = true; 	
		form.clo_start_add_ons.disabled = true; 
		form.clo_stop_add_ons.disabled = true; 
		
		
		
 	}
 	//exclude signal peptides	
// 	if(form.clo_offset_type.length>0)
 	if(typeof(form.clo_offset_type) != "undefined"){
 		if (form.clo_offset_type[1].checked)
 		{
 			// 		alert(alertMessageMature);
 			form.vstart_co.disabled = true;
 			form.vstart_co.checked=false;
 			form.clo_start_offset.value = 0;
 			form.clo_start_offset.disabled = true; 
//			form.clo_stop_offset.disabled = true; 	
 			form.clo_offset_type_start[0].disabled = true; 
			form.clo_offset_type_start[1].disabled = true; 
//			form.clo_offset_type_stop[0].disabled = true; 
//			form.clo_offset_type_stop[1].disabled = true; 	
 		}
 	}
 	if(!form.clo_addon_type[1].checked) 
 	{
 		form.clo_start_restrict.disabled = true;
		form.clo_stop_restrict.disabled = true; 
		form.clo_start_add_ons.disabled = true; 
		form.clo_stop_add_ons.disabled = true; 
 	}
 	if(!form.del_addon_type[3].checked) 
 	{
		form.del_start_add_ons.disabled = true; 
		form.del_stop_add_ons.disabled = true; 
 	}
}
function change_keio_file(form)
{    
	
	if(form.default_file[0].checked==true)
	{
//		jQuery('#download-keio-form div.form-item-files-userfile').addClass('form-disabled');
		form.userfile.disabled=true;	
		
	}
	else
	{
		form.userfile.disabled=false;
//		jQuery('#download-keio-form div.form-item-files-userfile').removeClass('form-disabled');
//		jQuery('#download-keio-form input.form-file').attr('disabled', false);
	}
}
function change_keio_report_type(form)
{    
	
	if(form.del_rbs[0].checked)
	{
		form.extention.value=0;
		form.extention.disabled=true;	
//		jQuery('#download-keio-form div.form-item-extention').addClass('form-disabled');
		
	}
	else
	{
		
		form.extention.disabled=false;
		form.extention.value=10;
//		jQuery('#download-keio-form div.form-item-extention').removeClass('form-disabled')
		
	}
}

function ShowThisSet(id)
{
	jQuery("div.query_set").hide();
	jQuery("#"+id).show(); 
}


function showPopup(id){
	jQuery('#venn_map div.popup').fadeOut(); 
	  var boxid = '#' + id + '-box';
	  jQuery(boxid).fadeIn();
	  jQuery('a.close').click(function(){
		  jQuery(this).parent().fadeOut();
	  });
}
function switchVennImage(url,url_bw)
{
	if( jQuery('#chk_venn_image').attr('checked') ){
		jQuery('#venn_map').css('backgroundImage','url('+url_bw+')');
	}else{
		jQuery('#venn_map').css('backgroundImage','url('+url+')');
	}
}
//function closePopup(id){
//	jQuery('#venn_map div.popup').fadeOut(); 
//	  var boxid = '#' + id + '-box';
//	  jQuery(boxid).fadeIn();
//	  jQuery('a.close').click(function(){
//	    $(this).parent().fadeOut();
//	  });
//}

