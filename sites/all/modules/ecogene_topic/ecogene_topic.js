/**
 * genepage script
 */


var array_topic_gene_set = function (base_url, aform, type)
{
	if (arguments.length == 2){
		aform.action = base_url + '/?q=ecosearch/gene/search';
	}else if(type=='array'){
		aform.action = base_url + '/?q=topic/array_topic_gene_query';
	}else if(type=='topic'){
		aform.action = base_url + '/?q=topic/genequery';
	}else if(type=='venn'){
		aform.action = base_url + '/?q=topic/genequery/venndiagram';
	}
	aform.submit();
}