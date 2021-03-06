<?php
//importando nuestras las referencias XAJAX
require ("xajax.common.php");


//funcion que lista las Particiones segun la IP elegida
function ListarOrigenMaster($ip){ 

	include_once("../includes/ctrlacc.php");
	include_once("../clases/AdoPhp.php");
	include_once("../includes/constantes.php");
	include_once("../includes/comunes.php");
	include_once("../includes/CreaComando.php");
	include_once("../includes/HTMLSELECT.php");
	
	
		//instanciamos el objeto para la respuesta AJAX
	 $objResponse = new xajaxResponse();	
	
	 $SelectHtml=" ";
	 $SelectHtml='<select name="source"> ';
	 
	 $cmd=CreaComando($cadenaconexion);
	 $rs=new Recordset; 
	 
	//Primera consulta: Particiones del MASTER potencialmente clonables.
    $cmd->texto='SELECT ordenadores_particiones.numdisk as DISK,ordenadores_particiones.numpar as PART,nombresos.nombreso as OS 
	FROM ordenadores_particiones INNER JOIN tipospar ON tipospar.codpar=ordenadores_particiones.codpar
	INNER JOIN nombresos ON ordenadores_particiones.idnombreso=nombresos.idnombreso 	
	INNER JOIN ordenadores ON ordenadores_particiones.idordenador=ordenadores.idordenador 
	WHERE ordenadores.ip="' .$ip . '"   
	AND tipospar.clonable>0  
	AND ordenadores_particiones.idnombreso>0
	ORDER BY ordenadores_particiones.numdisk,ordenadores_particiones.numpar';
	$rs->Comando=&$cmd; 

	if ($rs->Abrir()){
		$cantRegistros=$rs->numeroderegistros;
		if($cantRegistros>0){
			 $rs->Primero(); 
			while (!$rs->EOF){
				$SelectHtml.='<OPTION value=" '.$rs->campos["DISK"].' '.$rs->campos["PART"].'"';				
				$SelectHtml.='>';
				$SelectHtml.='DISK '.$rs->campos["DISK"].',PART '.$rs->campos["PART"].': '. $rs->campos["OS"].'</OPTION>';
				$rs->Siguiente();
			}
		}
		else
		{			
		$objResponse->alert("No partion found in this host for use it to cloning other computers.");
		}
		$rs->Cerrar();
	}
	//Segunda consulta: Imagenes del MASTER registradas como si fuese un repo.

#	$cmd->texto='SELECT *,repositorios.ip as iprepositorio FROM  imagenes
#INNER JOIN repositorios ON repositorios.idrepositorio=imagenes.idrepositorio
#where repositorios.ip="' .$ip .'"';

	$cmd->texto='select cache  from ordenadores_particiones where codpar = 202 and  idordenador = (SELECT idordenador from ordenadores where ip="' .$ip . '")';
	$rs->Comando=&$cmd;
	
	if ($rs->Abrir()){
		$cantRegistros=$rs->numeroderegistros;
		if($cantRegistros>0){
			$rs->Primero(); 
			while (!$rs->EOF){
				$files = explode(",", $rs->campos["cache"]);
				foreach ($files as $file) {
					if ( preg_match ( "/img$/", $file ) )  {					
					$imgname = rtrim($file, ".img");
					$SelectHtml.='<OPTION value=" CACHE /'.ltrim($imgname).'"';				
					$SelectHtml.='>';
					$SelectHtml.='IMG-CACHE: ' . ltrim($imgname).'</OPTION>';
					}
				}
				$rs->Siguiente();
			}
		}
		else
		{			
		$objResponse->alert("No image found in CACHE in this host for use it to cloning other compuers.");
		}
		$rs->Cerrar();
	}
	
//Tercera consulta: Imagenes del REPO, que el MASTER se encargara de enivarlas
	$cmd->texto='SELECT *,repositorios.ip as iprepositorio FROM  imagenes
INNER JOIN repositorios ON repositorios.idrepositorio=imagenes.idrepositorio
where repositorios.idrepositorio=(select idrepositorio from ordenadores where ordenadores.ip="' .$ip .'") ORDER BY imagenes.descripcion';
   
	$rs->Comando=&$cmd;
	
	if ($rs->Abrir()){
		$cantRegistros=$rs->numeroderegistros;
		if($cantRegistros>0){
			$rs->Primero(); 
			while (!$rs->EOF){
				$SelectHtml.='<OPTION value=" REPO /'.$rs->campos["nombreca"].'"';				
				$SelectHtml.='>';
				$SelectHtml.='IMG-REPO: ' . $rs->campos["descripcion"].'</OPTION>';
				$rs->Siguiente();
			}
		}
		else
		{			
		$objResponse->alert("No image found in REPO from this host for use it to cloning other computers.");
		}
		$rs->Cerrar();
	}
	
	$SelectHtml.= '</SELECT>';
 
	 //asignando el contenido de la varabiale $SelectHTML al div que esta en la paquina inicial
	 $objResponse->assign("divListado","innerHTML",$SelectHtml);

	 return $objResponse; //retornamos la respuesta AJAX
}
	
$xajax->processRequest(); //procesando cualquier peticion AJAX




?>
