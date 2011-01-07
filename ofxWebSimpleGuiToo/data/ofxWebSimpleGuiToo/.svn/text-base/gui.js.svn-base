

///////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL STUFF
///////////////////////////////////////////////////////////////////////////////////////////////////


// hash of all controls
var controls = new Array();
var mouseIsDown = false;
var guiSetUp = false;
function setupGui() {
	guiSetUp = true;
	$("body").mousedown(function(e) { mouseIsDown = true; });
	$("body").mouseup(function(e) { mouseIsDown = false; });
	
}



function createControl(data, parentId) {
	if(!guiSetUp) setupGui();
	if(data.type=="Title") data.value = data.name;
	if(data.type=="Content") return;
	try {
		eval('new '+data.type+'(data).render($("#'+parentId+'"));');
	} catch(e) {}
}


function map(val, min, max) {
	return min + (max-min)*val;
}
function unmap(val, min, max) {
	return (val-min)/(max-min);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// BASE CLASS
///////////////////////////////////////////////////////////////////////////////////////////////////



var Control = Class.extend({
	init: function(data) {
		this.data = data;
		this.name = data.name;
		this.id = data.name.replace(/ /g, '_');
		this.type = data.type;
		this.width = 0;
		this.height = 0;
		this.x = 0;
		this.y = 0;
		controls[this.id] = this;
		this.value = data.value;
	},
	

	setValue: function(val) {
		$("#"+this.id+"Value").html(": "+val);	
		$.get("/control?key="+this.name+"&value="+this.value);
	},
	
	render: function(parent) {
	
		// silly hack to stop the title drawing twice
		var title = '<h3>'+this.name+'<span id="'+this.id+'Value"></span></h3>';
		if(this.type=="Title") title = '';
		
		
		parent.append('<li>'+title
		+ '<div id="'+this.id+'" class="control '+this.type+'">'
		+ this.getControl()
		+'</div>'
		+'</li>');
		
		// get dimensions
		this.width = parseInt($("#"+this.id).css("width"));
		this.height = parseInt($("#"+this.id).css("height"));
		this.x  = $("#"+this.id).offset().left;
		this.y  = $("#"+this.id).offset().top;
		this.update();
		this.setValue(this.value);
		// enable events
		$("#"+this.id).mousedown(function(e) {

			controls[this.id].mousePressed(e.pageX-this.offsetLeft, e.pageY-this.offsetTop);
			controls[this.id].update();
			controls[this.id].setValue(controls[this.id].value);

		});
		
		$("#"+this.id).mouseup(function(e) {
			controls[this.id].mouseReleased(e.pageX-this.offsetLeft, e.pageY-this.offsetTop);
			controls[this.id].update();
			controls[this.id].setValue(controls[this.id].value);
		});
		
		$("#"+this.id).mousemove(function(e) {
			if(!mouseIsDown) controls[this.id].mouseMoved(e.pageX-this.offsetLeft, e.pageY-this.offsetTop);
			else {
				controls[this.id].mouseDragged(e.pageX-this.offsetLeft, e.pageY-this.offsetTop);
				controls[this.id].update();
				controls[this.id].setValue(controls[this.id].value);
			}
		});
	},
	
	


	// implement these!
	getControl: function() { return ''; },
	update: function() {},
	mousePressed: 	function(x, y) {},
	mouseReleased: 	function(x, y) {},
	mouseMoved: 	function(x, y) {},
	mouseDragged: 	function(x, y) {}
	
});

///////////////////////////////////////////////////////////////////////////////////////////////////
// SLIDER
///////////////////////////////////////////////////////////////////////////////////////////////////

var SliderFloat = Control.extend({
	init: function(data) {
		this._super(data);
		this.min = parseFloat(data.min);
		this.max = parseFloat(data.max);
		this.value = parseFloat(data.value);
	},
	getControl: function() {
		return '<div class="sliderHandle"></div>';
	},
	update: function() {
		var val = (this.value-this.min)/(this.max-this.min);
		val *= this.width;
		val += this.x;
		$("#"+this.id + " .sliderHandle").css("left", val + "px");
	},
	
	mousePressed: 	function(x, y) {
		var val = x / this.width;
		this.value = this.min + val*(this.max-this.min);
		if(this.value>this.max) this.value = this.max;
		if(this.value<this.min) this.value = this.min;
		
	},
	mouseDragged: 	function(x, y) {
		this.mousePressed(x,y);
	}
});



var SliderInt = SliderFloat.extend({
	mousePressed: 	function(x, y) {
		var val = x / this.width;
		this.value = Math.round(this.min + val*(this.max-this.min));
		if(this.value>this.max) this.value = this.max;
		if(this.value<this.min) this.value = this.min;
	}
});


///////////////////////////////////////////////////////////////////////////////////////////////////
// SLIDER 2D
///////////////////////////////////////////////////////////////////////////////////////////////////
var Slider2D = Control.extend({
	init: function(data) {
		this._super(data);
		this.minX = parseFloat(data.minX);
		this.maxX = parseFloat(data.maxX);
		this.minY = parseFloat(data.minY);
		this.maxY = parseFloat(data.maxY);
	},
	getControl: function() {
		return '<div class="slider2DHandle"></div>';
	},
	update: function() {


		var vals = this.value.split(",");


		var valX = (vals[0]-this.minX)/(this.maxX-this.minX);
		var valY = (vals[1]-this.minY)/(this.maxY-this.minY);
		valX *= this.width;
		valX += this.x;
		
		valY *= this.height;
		valY += this.y;
		$("#"+this.id + " .slider2DHandle").css("left", valX + "px");
		$("#"+this.id + " .slider2DHandle").css("top",  valY + "px");
	},
	
	mousePressed: 	function(x, y) {
		var valX = x / this.width;
		var valY = y / this.height;
		valX = this.minX + valX*(this.maxX-this.minX);
		valY = this.minY + valY*(this.maxY-this.minY);
		
		this.value = valX+","+valY;
	},
	mouseDragged: 	function(x, y) {
		this.mousePressed(x,y);
	}
});

///////////////////////////////////////////////////////////////////////////////////////////////////
// ColorPicker
///////////////////////////////////////////////////////////////////////////////////////////////////

var ColorPicker = Control.extend({
	
	getControl: function() {
		return '<div class="rHandle"></div><div class="gHandle"></div><div class="bHandle"></div><div class="aHandle"></div>';
	},
	update: function() {
		var vals = this.value.split(",");
		
		$("#"+this.id + " .rHandle").css("width", (parseFloat(vals[0])*this.width) + "px");
		$("#"+this.id + " .gHandle").css("width", (parseFloat(vals[1])*this.width) + "px");
		$("#"+this.id + " .bHandle").css("width", (parseFloat(vals[2])*this.width) + "px");
		$("#"+this.id + " .aHandle").css("width", (parseFloat(vals[3])*this.width) + "px");
	},
	
	mousePressed: 	function(x, y) {
		var valX = x / this.width;
		var valY = y / this.height;
		
		var vals = this.value.split(",");
		
		if(valY<0.25) vals[0] = valX;
		else if(valY<0.5) vals[1] = valX;
		else if(valY<0.75) vals[2] = valX;
		else vals[3] = valX;
		
		this.value = vals[0]+","+vals[1]+","+vals[2]+","+vals[3];
	},
	mouseDragged: 	function(x, y) {
		this.mousePressed(x,y);
	}
});


///////////////////////////////////////////////////////////////////////////////////////////////////
// Slider3D
///////////////////////////////////////////////////////////////////////////////////////////////////

var Slider3D = Control.extend({
	
	init: function(data) {
		this._super(data);
		this.minX = parseFloat(data.minX);
		this.maxX = parseFloat(data.maxX);
		this.minY = parseFloat(data.minY);
		this.maxY = parseFloat(data.maxY);
		this.minZ = parseFloat(data.minZ);
		this.maxZ = parseFloat(data.maxZ);

	},
	
	
	getControl: function() {
		return '<div class="xHandle"></div><div class="yHandle"></div><div class="zHandle"></div>';
	},
	
	
	update: function() {
		var vals = this.value.split(",");

		$("#"+this.id + " .xHandle").css("left", (this.width*unmap(parseFloat(vals[0]), this.minX, this.maxX)) + "px");
		$("#"+this.id + " .yHandle").css("left", (this.width*unmap(parseFloat(vals[1]), this.minY, this.maxY)) + "px");
		$("#"+this.id + " .zHandle").css("left", (this.width*unmap(parseFloat(vals[2]), this.minZ, this.maxZ)) + "px");
	},
	
	mousePressed: 	function(x, y) {
		var valX = x / this.width;
		var valY = y / this.height;
		
		var vals = this.value.split(",");
		
		if(valY<0.33) vals[0] = map(valX, this.minX, this.maxX);
		else if(valY<0.66) vals[1] = map(valX, this.minY, this.maxY);
		else vals[2] = map(valX, this.minZ, this.maxZ);
		
		this.value = vals[0]+","+vals[1]+","+vals[2];
	},
	
	mouseDragged: 	function(x, y) {
		this.mousePressed(x,y);
	}
});

///////////////////////////////////////////////////////////////////////////////////////////////////
// Button/Toggle
///////////////////////////////////////////////////////////////////////////////////////////////////

var Toggle = Control.extend({

	update: function() {
		$("#"+this.id).html(this.value=="true"?"on":"off");
	},
	
	mouseReleased: function() {
		this.value = this.value=="true"?"false":"true";
	}
});

var Button = Control.extend({

	update: function() {
		if(this.mouseIsPressed) $("#"+this.id).css("background-color", "#ff0000");
		else $("#"+this.id).css("background-color", "#000000");
	},
	
	mouseReleased: function() {
		this.mouseIsPressed = false;
	},
	
	mousePressed: function() {
		this.value = true;
		this.mouseIsPressed = true;
	}
});


var Title = Control.extend({
	getControl: function() {
		return "<h2>" + this.value + "</h2>";
	}
});

