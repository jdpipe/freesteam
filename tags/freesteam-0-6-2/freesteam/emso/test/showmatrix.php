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
$system=new System();

//-------------------------------------
// Read variable names from 'rlt' file
// (alternatively you can replace this section with just $variables=array();
// and the variables will be left un-named.)

$res =& readResults("RankineClosedLoopTest.rlt");

$connections=array(
#	array("PU.Out","BO.In")
	array("cyc.BO.Out","cyc.TU.In")
	,array("cyc.TU.Out","cyc.CO.In")
# 	,array("CO.Out","PU.In")
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
		// Equation coefficients line
		$e=$matches[1];
		
		if(!array_key_exists($e,$equations)){
			$equations[$e]="Equation ".$e;
			$eq2 =& new Equation("eq_".$e,$equations[$e]);
			$system->addEquation($eq2);
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
			
			if(!array_key_exists($variables[$v],$system->vars)){
				$var2 =& new Variable($variables[$v]);
				$system->addVariable($var2);
			}

			if(array_key_exists($v,$A[$e])){
				die("Error, element already defined for variable $v in equation $e: line is\n$line\n");
			}
			
			$A[$e][$v]=$matches1[$i][1];
			
			if(!is_a($eq2,"Equation")){die("Not an equation!");}
			
			$eq2->addVariable($variables[$v]);
		}
		
		
		//print_r($matches1);
	}elseif(preg_match("#^b\\s*:#",$line)){
		// Right-hand-side line:
		
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
		// Solutions line:
		
		list($head,$tail)=preg_split("#:#",$line);
		if(!preg_match_all("@\\s+(-?[0-9]\\.([0-9]+|#I0)e[+-][0-9]{3})@",$tail,$matches1,PREG_SET_ORDER)){
			die("Invalid 'x' line:\n$line\n");
		}
		for($i=0;$i<count($matches1);$i++){
			if(!array_key_exists($i,$variables)){
				$variables[$i]="x_".$i;
			}
			if(!array_key_exists($i,$variables2)){
				$var2=new Variable($variables[$i]);
				$system->addVariable($var2);
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

// Print equations as graph

print($system->toString());


$graphfp=fopen("equations.dot","w") or die("Couldn't write to equations.dot");

fwrite($graphfp,$system->toGraph());

fclose($graphfp) or die("Couldn't close equations.dot");



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
			
			print("Connecting $frompath to $topath...\n");
			
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


class System{
	var $vars;
	var $eqs;
	var $detvars;
	
	function System(){
		$this->vars=array();
		$this->eqs=array();
		$this->detvars=array();
	}
	
	function addEquation(&$equation){
		if(array_key_exists($equation->name,$this->eqs)){
			die("Equation name ".$equation->name.") already exists");
		}
		if(!is_a($equation,'Equation')){
			die("System::addEquation: not an equation");
		}
		
		//print("Adding ".$equation->name." to system\n");
		$this->eqs[$equation->name] =& $equation;
		$this->eqs[$equation->name]->system = & $this;
		//print("System equations: ".join(",",array_keys($this->eqs))."\n");
	}
	
	function addVariable(&$variable){
		if(array_key_exists($variable->name,$this->vars)){
			die("Variable named ".$variable->name.") already exists");
		}
		if(!is_a($variable,'Variable')){
			die("System::addVariable: not a variable");
		}
		//print("Adding ".$variable->name." to system\n");
		$this->vars[$variable->name] =& $variable;
		$this->vars[$variable->name]->system = & $this;
		//print("System variables: ".join(",",array_keys($this->vars))."\n");
	}
	
	function toString(){
	
		$s="SYSTEM:\n";
		$keys=array_keys($this->eqs);
		for($i=0;$i<count($keys);$i++){
			//print("printing equation ".$keys[$i]."\n");
			if(!is_a($this->eqs[$keys[$i]],'Equation')){
				die("System::toString: Not an equation!");
			}
				
			$s.=$this->eqs[$keys[$i]]->toString();
		}
		
		$keys=array_keys($this->vars);
		for($i=0;$i<count($keys);$i++){
			$s.=$this->vars[$keys[$i]]->toString();
		}
		$s.="END SYSTEM\n";
		return $s;
	}
		
	function toGraph(){	
		$this->traverseDetermined();
		
		$s="";
		print("Creating equations graph:\n\n");
		$s.="graph MYGRAPH{\n";
		$s.="\toverlap=false;\n";
		$s.="\tsplines=true;\n";
		$s.="\tmaxiter=1000;\n";
		$s.="\tnode [style=filled,color=gainsboro];\n";
		$keys=array_keys($this->vars);
		for($v=0;$v<count($this->vars);$v++){
			$s.=$this->vars[$keys[$v]]->toGraph();
		}
		$keys=array_keys($this->eqs);
		for($e=0;$e<count($this->eqs);$e++){
			if(!is_a($this->eqs[$keys[$e]],'Equation')){
				die("System::toGraph: not an equation");
			}
			$s.=$this->eqs[$keys[$e]]->toGraph();
		}
		$s.="\n}\n";
		return $s;
	}
	
	function &getVariable($name){
		if(!array_key_exists($name,$this->vars)){
			die("Invalid variable $name");
		}
		if(!is_a($this->vars[$name],'Variable')){
			print("System::getVariable: variables avail: ".join(", ",array_keys($this->vars))."\n");
			die("System::getVariable($name): not a variable, it's a ".gettype($this->vars[$name]));
		}
		return $this->vars[$name];
	}
	
	function &getEquation($name){
		//print("Getting equation $name\n");
		
		if(!array_key_exists($name,$this->eqs)){
			die("Invalid equation $name");
		}
		if(!is_a($this->eqs[$name],'Equation')){
			die("System::getEquation($name): not an equation");
		}
		return $this->eqs[$name];
	}
	
	function isDeterminedVar($name){
		return in_array($name,$this->detvars);
	}
	
	function setDeterminedVar($name){
		if(!$this->isDeterminedVar($name)){
			$this->detvars[]=$name;
		}
	}
	
	function traverseDetermined(){
	
		$keys=array_keys($this->eqs);
		for($i=0;$i<count($this->eqs);$i++){
			$equation =& $this->eqs[$keys[$i]];
			if($equation->isAssignment()){
				$variable =& $this->getVariable($equation->varnames[0]);
				if(in_array($variable->name,$this->detvars)){
					die("Variable overspecified: ".$variable->name);
				}
				print("Found assigned variable: ".$variable->name."\n");
				$this->detvars[]=$variable->name;
				$variable->assignedby=$equation->name;
				$variable->determined=true;
			}
		}
		
		$lastcount=0;
		$iteration=0;
		while(count($this->detvars)>$lastcount){
			$iteration++;
			print("Checking system, iteration $iteration...\n");	
			$lastcount=count($this->detvars);
			for($i=0;$i<count($this->detvars);$i++){
				$variable =& $this->getVariable($this->detvars[$i]);
				for($j=0;$j<count($variable->eqnames);$j++){
					$equation =& $this->getEquation($variable->eqnames[$j]);
					if($equation->isUnResolved()){
						print("Checking unresolved equation ".$equation->name."... ");
						$equation->checkDetermined();
					}
				}
			}
			print("... There are now ".count($this->detvars)." determined variables.\n");
		}
	}				
}		
	
/**
	Class to hold equations (relationships between variables)
*/
class Equation{
	var $varnames;
	var $name;
	var $expr;
	var $system;
	var $determined;
	var $been_here;
	var $resolved;
	
	function Equation($name,$expr=NULL){
		if($expr==NULL)$expr=$name;
		//print("Creating equation $name\n");
		
		$this->name=$name;
		$this->expr=$expr;
		$this->varnames=array();
		
		$this->determined=0;
		$this->been_here=false;
		$this->resolved=false;
	}
	
	function setSystem(&$system){
		$this->system =& $system;
	}
	
	function toString(){
		return "Equation '".$this->name."': ".join(", ",$this->varnames)."\n";
	}
	
	function addVariable($varname){
		
		if(in_array($varname,$this->varnames)){
			print($this->system->toString());
			
			die("Variable ".$varname." already in ".$this->name."\n");
		}
		
		$variable =& $this->system->getVariable($varname);
		
		$variable->addEquation($this->name);
		$this->varnames[] = $variable->name;
		
		//print("Assigned variable ".$variable->name." to equation ".$this->name." (now has ".count($this->varnames).")\n");
	}

	function toGraph(){
		$s="";
		$s.="\t$this->name [shape=box,label=\"$this->expr\"]";
		if($this->isAssignment()){
			$s.=" [color=yellow]";
		}elseif($this->isDetermined()){
			$s.=" [color=skyblue]";
		}
		$s.=";\n";
		
		for($i=0;$i<count($this->varnames);$i++){
			//print("Equation::toGraph: getting variable ".$this->varnames[$i]."\n");
			$variable =& $this->system->getVariable($this->varnames[$i]);
			$s.="\t".$this->name."--".$variable->getGraphName()."\n";
		}
		return $s;
	}
	
	function getVariableCount(){
		return count($this->varnames);
	}
	
	function isAssignment(){
		if(count($this->varnames)==1){
			$this->resolved=true;
			$this->determined=true;
			return true;
		}
		return false;
	}
	
	function isUnresolved(){
		return !$this->resolved;
	}
	
	function checkDetermined(){
		$debug=false;
		if($this->name=="eq_22")$debug=true;
		
		$determinedcount=0;
		for($i=0;$i<count($this->varnames);$i++){
			$variable =& $this->system->getVariable($this->varnames[$i]);
			if($variable->checkDetermined()){
				$determinedcount++;
			}
			if($debug)print("\n  Variable ".$variable->name." ".($variable->isDetermined() ? "is" : "is not")." determined");
		}
		
		if($debug)print("\n  Determinedcount: $determinedcount");
		
		if($determinedcount+1==count($this->varnames)){
			print("\nEquation '$this->name' is now determined");
			$this->resolved=true;	
			$this->determined=true;
			
			for($i=0;$i<count($this->varnames);$i++){
				$variable =& $this->system->getVariable($this->varnames[$i]);
				$variable->checkDetermined();
			}
		}else{
			print("(".$determinedcount."/".count($this->varnames).")\n");
			//print("Equation '$this->name': not determined, ".$determinedcount." vars det, total ".count($this->varnames)."\n");
			$this->determined=false;
		}
		
		return $this->determined;
	}
	
	function isDetermined(){
		return $this->determined;
	}
}

class Variable{
	var $name;
	var $value;
	var $eqnames;
	var $determined;
	var $assignedby;
	var $determinedby;
	var $system;
	
	function Variable($name){
		//print("Creating variable $name\n");
		$this->name=$name;
		$this->value=NULL;
		$this->eqnames=array();
		$this->determined=false;
		$this->assignedby=false;
		$this->determinedby="";
	}
	
	function setValue($value){
		$this->value=$value;
	}
	
	function setSystem(&$system){
		$this->system =& $system;
	}
	
	function toString(){
		return "Variable '".$this->name."': ".join(", ",$this->eqnames)."\n";
	}
	
	function toGraph(){
		$s="\t".$this->getGraphName()." [label=\"".$this->name."\"]";
		
		if(count($this->eqnames)==0){
			$s.=" [color=red]";
		}else{
			if($this->isAssigned()){
				$s.=" [color=green, label=\"".$this->name."\\nspec by: ".$this->assignedby."\"]";
			}elseif($this->isDetermined()){
				$s.=" [color=skyblue, label=\"".$this->name."\\ndet by: ".$this->determinedby."\"]";
			}
		}
		
		return $s.";\n";
	}
	
	function getGraphName(){
		return str_replace('.','_',$this->name);
	}
	
	function addEquation($eqname){
		if(in_array($eqname,$this->eqnames)){
			die("Equation ".$eqname." already assigned to ".$this->name);
		}

		//print("Assigned equation ".$eqname." to variable ".$this->name."\n");
		$this->eqnames[]= $eqname;
	}
	
	function checkDetermined(){
		if($this->isAssigned())return true;
		
		if($this->system->isDeterminedVar($this->name)){
			$this->determined=true;
			return true;
		}
		
		for($i=0;$i<count($this->eqnames);$i++){
			$equation =& $this->system->getEquation($this->eqnames[$i]);
			
			if($equation->isDetermined() && !$this->system->isDeterminedVar($this->name)){
				print("\nVariable ".$this->name." is now determined.\n");
				$this->determinedby=$equation->name;
				$this->system->setDeterminedVar($this->name);
				return true;
			}
		}
		
		return false;

	}		
	function isDetermined(){
		if(in_array($this->name,$this->system->detvars)){
			return true;
		}
		return false;
	}
	
	function isAssigned(){
		if($this->assignedby){
			return true;
		}
	}
	
	function getAssignedBy(){
		return $this->assignedby;
	}
	
	function setAssignedBy($eqname){
		$this->assignedby=$eqname;
	}
}

?>