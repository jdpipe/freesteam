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
$f = file("php://stdin");

$variables=array();
$variables[]="s.T";
$variables[]="s.p";
$variables[]="s.h";
$variables[]="s.s";
$variables[]="s.mdot";
$variables[]="BO.Out.T";
$variables[]="BO.Out.p";
$variables[]="BO.Out.h";
$variables[]="BO.Out.s";
$variables[]="BO.Out.mdot";
$variables[]="BO.q";
$variables[]="BO.Dp";

$equations=array();
$A=array();
$b=array();
$x=array();

//-------------------------------------
// No need to change from here onwards

reset($f);
while(list($n,$line)=each($f)){
	if(preg_match("#^([0-9]+)\\s*:(\\s+(-?[0-9]\\.[0-9]+e[+-][0-9]{3})\\(([0-9]+)\\))*\\s*$#",$line,$matches)){		
	
		$e=$matches[1];
		
		if(!array_key_exists($e,$equations)){
			$equations[$e]="Equation ".$e;
		}
		
		if(!array_key_exists($e,$A)){
			$A[$e]=array();
		}
		
		list($head,$tail)=preg_split("#:#",$line);
		
		if(!preg_match_all("#\\s+(-?[0-9]\\.[0-9]+e[+-][0-9]{3})\\(([0-9]+)\\)#",$tail,$matches1,PREG_SET_ORDER)){
			die("Invalid equation line\n$line\n");
		}
		
		for($i=0;$i<count($matches1);$i++){
			$v = $matches1[$i][2];
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
		if(!preg_match_all("#\\s+(-?[0-9]\\.[0-9]+e[+-][0-9]{3})#",$tail,$matches1,PREG_SET_ORDER)){
			die("Invalid 'b' line:\n$line\n");
		}
		for($i=0;$i<count($matches1);$i++){
			if(!array_key_exists($i,$equations)){
				$equations[$i]="Equation ".$i;
			}
			if(array_key_exists($i,$b)){
				die("Error, 'b' element already defined for variable $i");
			}
			
			$b[$i]=$matches1[$i][1];
		}
	}elseif(preg_match("#x\\s*:#",$line)){
		list($head,$tail)=preg_split("#:#",$line);
		if(!preg_match_all("#\\s+(-?[0-9]\\.[0-9]+e[+-][0-9]{3})#",$tail,$matches1,PREG_SET_ORDER)){
			die("Invalid 'x' line:\n$line\n");
		}
		for($i=0;$i<count($matches1);$i++){
			if(!array_key_exists($i,$variables)){
				$variables[$i]="x_".$i;
			}
			if(array_key_exists($i,$x)){
				die("Error, 'x' element already defined for variable $i");
			}
			
			$x[$i]=$matches1[$i][1];
		}
	}else{
		print("Didn't understand line:\n$line\n");
	}
	
}

// Print the equations

for($e=0;$e<count($equations);$e++){
	print($equations[$e].": ");
	reset($A[$e]);
	$first=true;
	while(list($v,$a)=each($A[$e])){
		if(!$first)print(" + ");
		print(($a+0.0)." * ".$variables[$v]);
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
				print(" ");
			}elseif($A[$e][$v]>0){
				print("+");
			}else{
				print("-");
			}
		}
		print("|\n");
	}
	print(" ");
	for($v=0;$v<count($variables);$v++){
		print("_");
	}
	print(" \n");

}else{
	print("\nSparsity matrix not printed: too many equations or variables (increase the limit if you want)\n");
}

print("\nReported solution:\n");
for($v=0;$v<count($variables);$v++){
	print($variables[$v]." = ".($x[$v]+0.0)."\n");
}
print("\n");

//print_r($A);

?>