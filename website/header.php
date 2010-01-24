<?php

DEFINE(DOCROOT,'/home/groups/f/fr/freesteam/htdocs');

if(!defined('DOCROOT')){
  define(DOCROOT,$SERVER['DOCUMENT_ROOT']);
}


$f=DOCROOT.$_SERVER['PHP_SELF'];
$t=filemtime($f);

header("Last-Modified: " . gmdate("D, d M Y H:i:s",$t) . " GMT");

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <title>freesteam: Steam Tables, Open Source, IAPWS-IF97</title>
  <link rel="stylesheet" media="screen" href="freesteam.css"
  <link rel="stylesheet" media="screen" href="highlight.css"
 type="text/css">
  <meta name="KEYWORDS"
 content="steam tables, IAPWS, steam properties, steam calculator, density, water, steam, IF97, saturated, superheated, subcooled, properties, enthalpy, entropy, specific heat, thermodynamics, thermo-fluids, IAPWS-95, python, ASCEND, EMSO, EES">
  <meta name="DESCRIPTION"
 content="Open source implementation of IAPWS-IF97 industry-standard steam tables in C++. Steam properties can also be calculated using included IAPWS-95 correlations.">
</head>
<body>
