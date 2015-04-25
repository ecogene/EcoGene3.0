/**
 * genepage script
 */
show_more_res = function() {
	
	
	 var containerUL = document.getElementById('more_res');
	 if (containerUL) {
			 containerUL.style.display = containerUL.style.display=='block'?'none':'block';
			 var containerLi = document.getElementById('more_res_li');
			 containerLi.className =  containerLi.className=='collapsed'? 'expanded':'collapsed';
	 }
	
};


function submitdnaform()
{
	
    document.forms["dnaForm"].submit();
}

function change_sites(checkbox, sites_checked_id)
{
	var  sites_checked = document.getElementById(sites_checked_id);
	var regex = new RegExp(checkbox.id+'(, |)', 'gi');	
	
	sites_checked.value = sites_checked.value.replace(regex,'');
	
//	alert(sites_checked.value);
	if(checkbox.checked)
	{
//		alert(checkbox.id);
		sites_checked.value = sites_checked.value + ',' + checkbox.id;
	}
//	alert(sites_checked.value);
	checkbox.form.submit();
}
function toggleBlockDiv(id, id_a)
{
	var container_a = document.getElementById(id_a);
	var containerDiv = document.getElementById(id);
	
	if(container_a.className.indexOf('cl_CollapsibleArea_collapsing') !=-1)
	{
		container_a.className =  'cl_CollapsibleArea_expanding';
		containerDiv.style.display = 'none';
	}
	else
	{	
		container_a.className =  'cl_CollapsibleArea_collapsing';
		containerDiv.style.display = 'block';
	
	}
	
	
		
}
function toggleProductInfoDiv(div_id)
{

 var elem = document.getElementById(div_id)

 elem.style.display= (elem.style.display=='none'? 'block':'none')
}
 
function toggleDiv(id,flagit) {

	if (flagit=="1"){

	if (document.layers) document.layers[''+id+''].visibility = "show"

	else if (document.all) document.all[''+id+''].style.visibility = "visible"

	else if (document.getElementById) document.getElementById(''+id+'').style.visibility = "visible"

	}

	else

	if (flagit=="0"){

	if (document.layers) document.layers[''+id+''].visibility = "hide"

	else if (document.all) document.all[''+id+''].style.visibility = "hidden"

	else if (document.getElementById) document.getElementById(''+id+'').style.visibility = "hidden"

	}

	}