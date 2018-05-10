获取元素的属性值
function getStyleValue(obj,attr){
	if(obj.currentStyle){ /*IE*/
		return obj.currentStyle[attr];
	}
	return getComputedStyle(obj,false)[attr]; /*firefox chrome*/
}