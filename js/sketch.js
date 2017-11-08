function RandInt(min, max) {
  return Math.floor(Math.random() * (max - min) + min);
}

function genGraph(){
	var cnt = 0;

	for (var i = 1; i < n; ++i){
		var ind = RandInt(0, i);

		mat[i][ind] = 1;
		mat[ind][i] = 1;
	}

	for (var i = 0; i < n; ++i){
		var num = RandInt(0, n**(1/4));

		while(num--){
			var ind = RandInt(0, n);

			if (ind != i){
				mat[i][ind] = 1;
				mat[ind][i] = 1;
    		}
		}
	}

	for (var i = 0; i < n; ++i)
		for (var k = i + 1; k < n; ++k)
			if (mat[i][k] == 1){
				edges[cnt] = {from:i, to:k};
        		cnt++;
        	}
}

function numComp(matrix){
	var m = 0, t;

	var cands = new Set();
	cands.add(0);

	for (let i of cands){
		t = matrix[i].indexOf(1);

		while (t != -1){
			++m;

			matrix[i][t] = 0;
			matrix[t][i] = 0;

			cands.add(t);
			t = matrix[i].indexOf(1);
		}
	}
	return cands.size == n;
}

var n = 42;

var mat = new Array(n);

var nodes = [];
var edges = [];

for (var i = 0; i < n; ++i){
	mat[i] = new Array(n).fill(0);
    nodes[i] = {id:i, group: 1};
}

genGraph();


//console.log(numComp(mat));
//console.log(mat);


var arr = [];

for (var i = 0; i < edges.length; ++i){

	for (var j = 0; j < n; ++j)
    	arr[j] = mat[j].slice();

    var c = edges[i];

    arr[c.to][c.from] = 0;
    arr[c.from][c.to] = 0;

    if (!numComp(arr))
    	edges[i].color = 'red';
}
/*
for (var i = 0; i < edges.length; ++i){
	if (RandInt(0,2))
		clr = 'red';
	else
		clr = 'blue';

	edges[i].color = clr;
}
*/

var container = document.getElementById('mynetwork');

var data = {
    nodes: nodes,
    edges: edges
};

var options = {
    nodes: {
        shape: 'dot',
        size: 14,
        borderWidth: 2
    },
    edges: {
        width: 2
    },
    physics: false,
    smooth:{
    	enabled: false
    },
    length: 40
};

network = new vis.Network(container, data, options);