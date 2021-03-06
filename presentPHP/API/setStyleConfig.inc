<?php
/* Used to genrate API/cofig/*style*.json for all supported styles
which will be used by parser.inc

- API/supported_style.json contains a list of supported sytle,
  > for every style, there is a few key-value pairs,
  > keys = [paradigm, format, dependFolders, dependenceDetails];

- API/config/head-body contains *style*_body/head.in
  > the files will be loaded to set *style*[body/head] in *style*.json
  > *style*[head/body] will uesd by parser.inc/setSpecificStyle(),
  
  > $specificHEAD = "$basicHEAD\n $styleHEAD\n";// Apped directly
  
  > $specificBODY = str_replace("<structuredBODY/>", $structuredBODY, $styleBODY);

- API/*style*.json : every style must have its own .json
  > key = ['name', 'oaradimg', 'head', 'body', 'format', 'dependence']
  > 'dependence' is the connected file list of every folder 
  > in supported_style.json[*style*][dependFolders]


- To add/update a new style
  1. add/update the key-value pairs in  supported_style.json
  2. If the style need some offline js/css fils
  	 - create a folder API/*Style*
  	 - includs all the filels in that folder
  	 - Add "dependence":["*style*\/"], in supported_style.json
  3. if need, add *style*_head.in including the specific head parts
  4. if need, add *style*_body.in & <structuredBODY/> will be replace by $structuredBODY;
  5. execute this function : setStyleConfig.inc, 
     all the *style*.json will be updated,
     and new *style*.json will be added.
  6. re-init the Parser() to refresh its $support_style list

*/ 
	$API_FOLDER = __DIR__;
	$CONFIG_FOLDER = "config/xml";//
	$HB_FOLDER = "config/head_body";//

	$supportedStyle = "supported_style.json";
	$supportedStyle = file_get_contents($API_FOLDER."/".$supportedStyle);
	$supportedStyle = json_decode($supportedStyle, TRUE);

	foreach ($supportedStyle as $name => $style) {

		$styleHEAD = file_get_contents($API_FOLDER."/".$HB_FOLDER."/".$name."_head.in");
		$styleBODY = file_get_contents($API_FOLDER."/".$HB_FOLDER."/".$name."_body.in");
		// echo $API_FOLDER."/"."beta"."/".$name."_head.in";
		$styleHEAD = ($styleHEAD) ? $styleHEAD : '' ;
		$styleBODY = ($styleBODY) ? $styleBODY : '' ;

		$depDetails = array();
		foreach ($style['dependFolders'] as $depFolder) {
			$depTemp = $API_FOLDER."/"."../".$depFolder;
			$depTemp =array_slice(scandir($depTemp, 1), 0, -2);
			foreach ($depTemp as &$value) {
				if ($value!=".DS_Store") {
					$value = $depFolder.'/'.$value;
				} else{
					$value = "";
				}
			}
			$depDetails = array_merge($depDetails, $depTemp);
			unset($depTemp);
		}
		$style['dependence'] = $depDetails;

		$style = array(
			'name' => $name, 
			'paradigm' => $style['paradigm'],
			'head' => $styleHEAD,
			'body' => $styleBODY,
			'format' =>$style['format'],
			'dependence' =>$style['dependence']
			);
		$filename = $API_FOLDER."/"."config"."/".$name.".json";
		file_put_contents($filename, json_encode($style, JSON_PRETTY_PRINT));

		// var_dump($style);
	}

?>