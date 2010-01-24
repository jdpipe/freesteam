<?php

$latestversion ="2.0";
$releasedate = "15 Oct 2007";

$viewsvn_base = "http://freesteam.svn.sourceforge.net/viewvc/freesteam/trunk/";

$download_page = "http://sourceforge.net/projects/freesteam/files/";

$freesteam_home = "/";

class Link{
	function Link($link,$url,$tip=NULL){
		$this->url=$url;
		$this->link=$link;
		$this->tip=$tip;
	}
	function toString(){
		$str = "<a href=\"".htmlspecialchars($this->url)."\"";
		if($this->tip) $str .= " title=\"".htmlspecialchars($this->link)."\"";
		$str .= ">".htmlspecialchars($this->link);
		return $str."</a>";
	}
}

function show_nav($this_page=NULL,$current_page=NULL){
	global $latestversion, $releasedate, $viewsvn_base, $download_page, $freesteam_home;

	print "<table><tr><td valign=top><div class=\"nav\">";

	if($current_page!=NULL){
		print "<a href=\"".htmlspecialchars($freesteam_home)."\">&larr; freesteam home</a><br>";
	}else{
		print "<span style=\"font-weight: bold;\">Current version: </span><a href=\"".htmlspecialchars($download_page)."\">".htmlspecialchars($latestversion)."</a><br>";
		
		print "<span style=\"font-weight: bold;\">Released: </span>".htmlspecialchars($releasedate)."<br>";
	}
	
	$links = array(
		new Link("Examples", "example.php", "Code samples")
		,new Link("Download", $download_page, "File releases at SourceForge")
		,new Link("How to compile", "compile.php", "Instructions for building the software from source")
		,new Link("Support", "https://lists.sourceforge.net/lists/listinfo/freesteam-users", "Get support for Freesteam (mailing list)")
		,new Link("Changelog", "${viewsvn_base}CHANGELOG.txt?only_with_tag=HEAD&amp;view=markup", "via ViewVC on SourceForge")
		,new Link("Report bugs", "http://ascendbugs.cheme.cmu.edu/set_project.php?project_id=3", "Mantis bug tracker at cmu.edu")
		,new Link("Browse SVN", $viewsvn_base, "ViewVC at SourceForge")
		,new Link("Code docs", "docs/", "Code docs generated with Doxygen")
		,new Link("Property diagrams", "properties/")
		,new Link("Steam tables", "http://downloads.sourceforge.net/project/freesteam/freesteam/2.0/freesteam-steam-tables.pdf")
		#,new Link("BuildBot","http://buildbot.cruncher2.dyndns.org/")
		,new Link("SourceForge",	"http://sourceforge.net/projects/freesteam/", "Project page")
	);

	print "<ul>";
	foreach($links as $l){
		if($l->link==$current_page){
			print "  <li><b>".$l->link."</b></li>\n";
		}else{
			print "  <li>".$l->toString()."</li>\n";
		}
	}
	print "</ul>";

	if($this_page){
		print "On this page...\n";
		print "<ul>";
		foreach($this_page as $l){
			print "  <li>".$l->toString()."</li>\n";
		}
		print "</ul>";
	}

    print "<SCRIPT type='text/javascript' language='JavaScript' src='http://ohloh.net/projects/3964;badge_js'></SCRIPT>";

	print "</div></td><td valign=top>";

}
