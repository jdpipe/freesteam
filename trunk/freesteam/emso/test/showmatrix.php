<?php
/**
	You can use this file to interpret the output panel from EMSO. Copy the text corresponding to one iteration of the NLASOlver, the run this script, feeding it that text either from a file or from stdin.
	
	You will then get the matrix converted into equation form, accompanied by a sparsity matrix and the reported solution.
	
	If you want, you can name the variables and equations in the first section of the file; these names will be used when printing the equations / solution.

	Example of satisfactory input to this script: (remove tabs at starts of lines)
	
	0 : -4.197e+004(0)  -1.134e+001(1)  1.000e+000(2)  
	1 : -1.447e+002(0)  -3.925e-002(1)  1.000e+000(3)  
	2 : 1.000e+000(5)  2.198e-007(6)  -2.393e-004(7)  
	3 : -5.038e-002(6)  -3.494e-001(7)  1.000e+000(8)  
	4 : -1.000e+000(4)  1.000e+000(9)  
	5 : 1.000e+000(2)  1.135e+005(4)  -1.000e+000(7)  -1.165e+005(9)  1.000e+000(10)  
	6 : 1.000e+000(1)  -1.000e+000(6)  1.000e+000(11)  
	7 : 1.000e+000(4)  
	8 : 1.000e+000(0)  
	9 : 1.000e+000(1)  
	10 : 1.000e+000(11)  
	11 : 1.000e+000(10)  
	Linear system solution number 25
	b :   0.000e+000  0.000e+000  1.705e-013  1.478e-012  0.000e+000  -2.000e+003  0.000e+000  0.000e+000  0.000e+000  0.000e+000  0.000e+000  -2.000e+003
	x :   0.000e+000  0.000e+000  0.000e+000  0.000e+000  0.000e+000  1.705e-013  0.000e+000  0.000e+000  1.478e-012  0.000e+000  -2.000e+003  0.000e+000
	
*/

//-------------------------------------

$equations=array();
$A=array();
$b=array();
$x=array();

//-------------------------------------
// Read variable names from 'rlt' file
// (alternatively you can replace this section with just $variables=array();
// and the variables will be left un-named.)

$res =& readResults("cycletest2.rlt");

$connections=array(
	array("PU.Out","BO.In")
	,array("BO.Out","TU.In")
	,array("TU.Out","CO.In")
	,array("CO.Out","PU.In")
);

$res->applyConnections($connections);

$res->addIndepNodes();

$variables =& $res->getIndepVarList();

//-------------------------------------
// Read equations from stdin

$f = file("php://stdin");


//-------------------------------------
// No need to change from here onwards

reset($f);
while(list($n,$line)=each($f)){
	if(preg_match("@^([0-9]+)\\s*:(\\s+(-?[0-9]\\.([0-9]+|#I0+)e[+-][0-9]{3})\\(([0-9]+)\\))*\\s*$@",$line,$matches)){		
	
		$e=$matches[1];
		
		if(!array_key_exists($e,$equations)){
			$equations[$e]="Equation ".$e;
		}
		
		if(!array_key_exists($e,$A)){
			$A[$e]=array();
		}
		
		list($head,$tail)=preg_split("#:#",$line);
		
		if(!preg_match_all("@\\s+(-?[0-9]\\.([0-9]+|#I0+)e[+-][0-9]{3})\\(([0-9]+)\\)@",$tail,$matches1,PREG_SET_ORDER)){
			die("Invalid equation line\n$line\n");
		}
		
		for($i=0;$i<count($matches1);$i++){
			$v = $matches1[$i][3];
			if(!array_key_exists($v,$variables)){
				$variables[$v]="x_".$v;
			}
			if(array_key_exists($v,$A[$e])){
				die("Error, element already defined for variable $v in equation $e: line is\n$line\n");
			}
			
			$A[$e][$v]=$matches1[$i][1];
		}
		
		//print_r($matches1);
	}elseif(preg_match("#^b\\s*:#",$line)){
		list($head,$tail)=preg_split("#:#",$line);
		if(!preg_match_all("@\\s+(-?[0-9]\\.([0-9]+|#I0+)e[+-][0-9]{3})@",$tail,$matches1,PREG_SET_ORDER)){
			die("Invalid 'b' line:\n$line\n");
		}
		for($i=0;$i<count($matches1);$i++){
			if(!array_key_exists($i,$equations)){
				$equations[$i]="Equation ".$i;
			}
			if(array_key_exists($i,$b)){
				die("Error, 'b' element already defined for variable $i (check you don't have multiple 'b:' lines)");
			}
			
			$b[$i]=$matches1[$i][1];
		}
	}elseif(preg_match("#x\\s*:#",$line)){
		list($head,$tail)=preg_split("#:#",$line);
		if(!preg_match_all("@\\s+(-?[0-9]\\.([0-9]+|#I0)e[+-][0-9]{3})@",$tail,$matches1,PREG_SET_ORDER)){
			die("Invalid 'x' line:\n$line\n");
		}
		for($i=0;$i<count($matches1);$i++){
			if(!array_key_exists($i,$variables)){
				$variables[$i]="x_".$i;
			}
			if(array_key_exists($i,$x)){
				die("Error, 'x' element already defined for variable $i (check you don't have multiple 'x:' lines)");
			}
			
			$x[$i]=$matches1[$i][1];
		}
	}else{
		print("Didn't understand line:\n$line\n");
	}
	
}

// Print the equations

print ("\nEquations:\n\n");

for($e=0;$e<count($equations);$e++){
	//print($equations[$e].": ");
	reset($A[$e]);
	$first=true;
	while(list($v,$a)=each($A[$e])){
		if($a+0.0 < 0){
			if($first){
				print("-");
			}else{
				print(" - ");
			}
		}else{
			if(!$first)
				print(" + ");
		}
		
		if(abs($a+0.0)!=1.0){
			print(abs($a+0.0)." * ");
		}
		print($variables[$v]);
		$first=false;
	}
	print(" = ".($b[$e]+0.0)."\n");
}

if(count($variables) < 50 && count($equations) < 50){

	print("\nSparsity matrix:\n");

	print(" ");
	for($v=0;$v<count($variables);$v++){
		print("_");
	}
	print(" \n");

	for($e=0;$e<count($equations);$e++){
		print("|");
		for($v=0;$v<count($variables);$v++){
			if(!array_key_exists($v,$A[$e]) || $A[$e][$v]==0){
				if($e==count($equations)-1){
					print("_");
				}else{
					print(" ");
				}
			}elseif($A[$e][$v]>0){
				print("+");
			}else{
				print("-");
			}
		}
		print("|\n");
	}

}else{
	print("\nSparsity matrix not printed: too many equations or variables (increase the limit if you want)\n");
}

if(count($x)==count($variables)){
	print("\nReported solution:\n");
	for($v=0;$v<count($variables);$v++){
		print($variables[$v]." = ".($x[$v]+0.0)."\n");
	}
	print("\n");
}else{
	print("\n\nReported solution was invalid\n");
}



//-------------------------------------------
// Classes to hold results for EMSO '*.rlt' files

/**
	Results node: could be either a value node, a model node, or a flowsheet node
*/
class Node{
	var $name;
	var $parent;
	var $connections; // ie aliased nodes, which containing identical information
	
	function Node($name){
		$this->name=$name;
		$this->connections=array();
		$this->parent=NULL;
		$this->path=$this->name;
	}
	
	function setParent(&$parent){
		$this->parent=$parent;
		
		if($parent->path){
			$this->path=$parent->path.".".$this->name;
		}else{
			$this->path=$this->name;
		}
	}
	
	function &newValueNode($name){
		$n = new ValueNode($name);
		$n->setParent($this);
		$this->nodes[$name] =& $n;
		return $n;
	}

	function &newModelNode($name){
		$n = new ModelNode($name);
		$n->setParent($this);
		$this->nodes[$name] =& $n;
		return $n;
	}

	function addVar($name,$val){
		$n =& $this->newValueNode($name);
		$n->value=$val;
	}
	
	function getPath(){
		return $this->path;
	}
	
	function &getFlowSheet(){
		if(is_a($this,"FlowSheet")){
			return $this;
		}else{
			return $this->parent->getFlowSheet();
		}
	}
		
	function read(&$lines){
		$numvars=trim(array_shift($lines));
		
		//print("Vars: $numvars\n");
		for($i=0;$i<$numvars;$i++){
			$varname=trim(array_shift($lines));
			
			//print("Var name: $varname\n");
			
			$data=explode(" ",array_shift($lines));
			$varvalue=$data[3];
			
			$newvar =& $this->newValueNode($varname);
			$newvar->value=$varvalue;
			
			//print("New var: ".$newvar->getPath()."\n");
		}
		
		$numnodes=array_shift($lines);
		for($i=0;$i<$numnodes;$i++){
			$nodename=trim(array_shift($lines));
			
			//print("Node name: $nodename\n");
			
			$node =& $this->newModelNode($nodename);
			$node->read($lines);
		}
	}
	
	function toString($prefix=NULL){
		
		foreach($this->nodes as $no){
			$no->toString($prefix.$no->name.".");
		}
	}
	
	function &findByPathSplit($path){
		if(count($path)==0){
			return $this;
		}
		
		$first=array_shift($path);
		if(array_key_exists($first,$this->nodes)){
			return $this->nodes[$first]->findByPathSplit($path);
		}else{
			return NULL;
		}
	}
	
	function applyConnection(&$to){
		if(!is_array($this->connections)){
			die("Not an array");
		}
		if(!is_a($to,"Node")){
			die("Not a node");
		}
		
		//print("Connecting from ".$this->getPath()."...\n");
		//print("Connecting to ".$to->getPath()."...\n");
		
		for($i=0;$i<count($this->connections);$i++){
			print("Checking ".$this->connections[$i]->getPath()."\n");
			if($this->connections[$i]==$to){
				die("Already connected");
			}
		}
		
		$this->connections[] =& $to;
	}
	
	function isAliasOf(&$to){
		
		//print("Checking ".$this->getPath()." alias of ".$to->getPath()."...\n");
		
		if(count($this->connections)){
			
			//print("Checking ".count($this->connections)." connections\n");
						
			for($i=0;$i<count($this->connections);$i++){
				if($this->connections[$i]->path==$to->path){
					//print("Found ".$this->getPath()." alias of ".$to->getPath()."...\n");
					return true;
				}
			}
		}
		
		if($this->name!=$to->name){
			return false;
		}
		
		if(!$this->parent || $to->parent){
			return false;
		}

		//print("Checking parents... ".$this->getPath()." alias of ".$to->getPath()."...\n");
		
		return $this->parent->isAliasOf($to->parent);
	}
	
	function addIndepNodes(&$flowsheet){
		
		for($j=0;$j<count($flowsheet->indepnodes);$j++){
			if($this->isAliasOf($flowsheet->indepnodes[$j]))return;			
		}
		
		//print("Adding ".$this->getPath()."\n");
		$flowsheet->indepnodes[] =& $this;
		
		if(is_a($this,"ModelNode")){
			$keys=array_keys($this->nodes);
			for($i=0;$i<count($keys);$i++){
				//print($this->nodes[$keys[$i]]->getPath()."...\n");
				$this->nodes[$keys[$i]]->addIndepNodes($flowsheet);
			}
		}
	}	
}

/**
	Value node also has a value
*/
class ValueNode extends Node{
	var $value;
	
	function ValueNode($name){
		$this->Node($name);
		$this->value=NULL;
	}
	
	function toString($prefix=NULL){
		return("$prefix.$n = ".$v."\n");
	}

}

/**
	Model node can have child nodes
*/
class ModelNode extends Node{
	var $nodes;

	function toString($prefix=NULL){	
		$str="";
		$keys=array_keys($this->nodes);
		for($i=0;$i<count($keys);$i++){
			$str.=$this->nodes[$keys[$i]]->toString();
		}
	}	
}

/**
	Flowsheet node doesn't put its name into the path
	Names of all the independent variables can be stored here too.
*/
class FlowSheet extends ModelNode{
	var $indepnodes;
	
	function getPath(){
		return NULL;
	}
	
	function FlowSheet($name){
		$this->ModelNode($name);
		$this->indepvars=array();
		$this->path="";
	}
	
	function toString($prefix=NULL){
		foreach($this->vars as $n=>$v){
			print($n." = ".$v."\n");
		}
		foreach($this->nodes as $no){
			$no->toString();
		}
	}
	
	function &findByPath($path){
		$path1=explode(".",$path);
		return $this->findByPathSplit($path1);
	}

	function applyConnections($connections){
		
		for($i=0;$i<count($connections);$i++){
			if(count($connections[$i])!=2){
				die("Invalid connections");
			}
			$frompath=$connections[$i][0];
			$topath=$connections[$i][1];
			
			//print("Connecting $frompath to $topath...\n");
			
			$from =& $this->findByPath($frompath);
			$to =& $this->findByPath($topath);
			
			if($from==NULL){
				die("Invalid 'from'");
			}
			if($to==NULL){
				die("Invalid 'to'");
			}
			
			$from->applyConnection($to);
			$to->applyConnection($from);
		}
	}
	
	function addIndepNodes(){
		parent::addIndepNodes($this);
	}
	
	function &getIndepVarList(){
		$vars=array();
		for($i=0;$i<count($this->indepnodes);$i++){
			if(is_a($this->indepnodes[$i],"ValueNode")){
				//print("Variable ".$this->indepnodes[$i]->getPath()."\n");
				$vars[]=$this->indepnodes[$i]->getPath();
			}
		}
		return $vars;
	}
}

/**
	Read and parse a *.rlt file from EMSO
*/
function readResults($filename){
	
	$lines = file($filename);
	
	$flowsheetname=trim(array_shift($lines));
	$rubbish=array_shift($lines);
	
	$node =& new FlowSheet($flowsheetname);
	$node->read($lines);
	
	return $node;
}

?>