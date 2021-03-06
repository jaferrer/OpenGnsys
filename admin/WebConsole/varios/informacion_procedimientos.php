<?php
// *************************************************************************************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2009-2010
// Fecha Última modificación: Agosto-2010
// Nombre del fichero: informacion_procedimientos.php
// Descripción : 
//		Muestra los comandos que forman parte de un procedimiento y sus valores
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/CreaComando.php");
include_once("../clases/XmlPhp.php");
include_once("../clases/ArbolVistaXML.php");
include_once("../idiomas/php/".$idioma."/informacion_procedimientos_".$idioma.".php");
//________________________________________________________________________________________________________
$idprocedimiento=0; 
$descripcionprocedimiento=""; 
if (isset($_GET["idperfil"])) $idperfil=$_GET["idperfil"]; // Recoge parametros
if (isset($_GET["descripcionprocedimiento"])) $descripcionprocedimiento=$_GET["descripcionprocedimiento"]; // Recoge parametros

$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexióncon servidor B.D.
else
	$arbolXML=CreaArbol($cmd,$idperfil); // Crea el arbol XML 

// Creación del árbol
$baseurlimg="../images/tsignos";
$clasedefault="tabla_listados_sin";
$titulotabla=$TbMsg[3];  
$arbol=new ArbolVistaXml($arbolXML,0,$baseurlimg,$clasedefault,1,20,130,1,$titulotabla);
//________________________________________________________________________________________________________
?>
<HTML>
<HTML>
<TITLE>Administración web de aulas</TITLE>
<HEAD>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
	<LINK rel="stylesheet" type="text/css" href="../estilos.css">
	<SCRIPT language="javascript" src="../clases/jscripts/ArbolVistaXML.js"></SCRIPT>
</HEAD>
<BODY>
	<P align=center class=cabeceras><?php echo $TbMsg[0]?><BR>
	<SPAN align=center class=subcabeceras><?php echo $TbMsg[1]?></SPAN>&nbsp;<IMG src="../images/iconos/confisoft.gif"><BR><BR>
	<IMG src="../images/iconos/procedimiento.gif"><SPAN class=presentaciones>&nbsp;&nbsp;<U><?php echo $TbMsg[2]?></U>:	<?php echo $descripcionperfil?></SPAN></P>
	<?php echo $arbol->CreaArbolVistaXml(); // Crea arbol de configuraciones?>
</BODY>
</HTML>
<?php
/**************************************************************************************************************************************************
	Devuelve una cadena con formato XML de toda la Información de los perfiles software
	softwares
	Parametros: 
		- cmd:Una comando ya operativo ( con conexiónabierta)  
		- idperfil: El identificador del perfil software
________________________________________________________________________________________________________*/
function CreaArbol($cmd,$idperfil){
	$cadenaXML=SubarbolXML_procedimientoswares($cmd,$idperfil);
	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_procedimientoswares($cmd,$idperfilsoft)
{
	global $TbMsg;

	$cadenaXML="";

	$cmd->texto="SELECT procedimientos.idperfilsoft ,procedimientos.descripcion as pdescripcion, procedimientos.comentarios,
								softwares.idsoftware,softwares.descripcion as hdescripcion,tiposoftwares.urlimg FROM procedimientos  
								LEFT OUTER JOIN  procedimientos_softwares  ON procedimientos.idperfilsoft=procedimientos_softwares.idperfilsoft
								LEFT OUTER JOIN  softwares  ON softwares.idsoftware=procedimientos_softwares.idsoftware
								LEFT OUTER JOIN  tiposoftwares  ON softwares.idtiposoftware=tiposoftwares.idtiposoftware
								WHERE procedimientos.idperfilsoft=".$idperfilsoft."
								ORDER by tiposoftwares.idtiposoftware,softwares.descripcion";
	$rs=new Recordset; 								
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	$cadenaXML.='<procedimientosWARES';
	// Atributos
	$cadenaXML.=' imagenodo="../images/iconos/procedimiento.gif"';
	$cadenaXML.=' infonodo="'.$rs->campos["pdescripcion"].'"';
	$cadenaXML.='>';
	if($rs->campos["comentarios"]>" "){
		$cadenaXML.='<PROPIEDAD';
		$cadenaXML.=' imagenodo="../images/iconos/propiedad.gif"';
		$cadenaXML.=' infonodo="[b]'.$TbMsg[8].' :[/b] '.$rs->campos["comentarios"].'"';
		$cadenaXML.='>';
		$cadenaXML.='</PROPIEDAD>';
	}
	$swcompo=false;
	while (!$rs->EOF){
		if ($rs->campos["idsoftware"]){
			if (!$swcompo) {
				$cadenaXML.='<COMPONENTES';
				$cadenaXML.=' imagenodo="../images/iconos/carpeta.gif"';
				$cadenaXML.=' infonodo="'.$TbMsg[6].'"';
				$cadenaXML.='>';
				$swcompo=true;
			}	
			$cadenaXML.='<procedimiento';
			// Atributos
			$cadenaXML.=' imagenodo='.$rs->campos["urlimg"];
			$cadenaXML.=' infonodo="'.$rs->campos["hdescripcion"].'"';
			$cadenaXML.='>';
			$cadenaXML.='</procedimiento>';
		}
		$rs->Siguiente();
	}
	if ($swcompo) {
		$cadenaXML.='</COMPONENTES>';
	}
	$cadenaXML.=SubarbolXML_Ordenadores($cmd,$idperfilsoft);
	$cadenaXML.=SubarbolXML_ImagenesDisponibles($cmd,$idperfilsoft);
	$cadenaXML.='</procedimientosWARES>';
	$rs->Cerrar();
	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_Ordenadores($cmd,$idperfilsoft)
{
	global $TbMsg;

	$cadenaXML="";
	$gidaula=0;
	$cmd->texto="SELECT DISTINCT aulas.idaula,aulas.nombreaula,ordenadores.idordenador,
								ordenadores.nombreordenador,ordenadores_particiones.numpar
								FROM ordenadores
 								INNER JOIN aulas ON  ordenadores.idaula=aulas.idaula
								INNER JOIN ordenadores_particiones ON  ordenadores_particiones.idordenador=ordenadores.idordenador 								
 								WHERE ordenadores_particiones.idperfilsoft=".$idperfilsoft." ORDER BY aulas.idaula,ordenadores.nombreordenador";
	$rs=new Recordset; 
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	$rs->Primero(); 
	if ($rs->numeroderegistros>0){
		$cadenaXML.='<ORDENADORES';
		$cadenaXML.=' imagenodo="../images/iconos/carpeta.gif"';
		$cadenaXML.=' infonodo="'.$TbMsg[7].'"';
		$cadenaXML.='>';
	}
	while (!$rs->EOF){
		if ($gidaula!=$rs->campos["idaula"]){
			if (!empty($gidaula))
				$cadenaXML.='</AULA>';
			$cadenaXML.='<AULA ';
			// Atributos		

			$cadenaXML.=' imagenodo="../images/iconos/aula.gif"';
			$cadenaXML.=' infonodo="'.$rs->campos["nombreaula"].'"';
			$cadenaXML.='>';
			$gidaula=$rs->campos["idaula"];
		}
		$cadenaXML.='<ORDENADOR';
		// Atributos			
		$cadenaXML.=' imagenodo="../images/iconos/ordenador.gif"';
		$cadenaXML.=' infonodo="'.$rs->campos["nombreordenador"].'"' ;
		$cadenaXML.='></ORDENADOR>';
		$rs->Siguiente();
	}
	if (!empty($gidaula))
		$cadenaXML.='</AULA>';
	if ($rs->numeroderegistros>0)
			$cadenaXML.='</ORDENADORES>';
	$rs->Cerrar();
	return($cadenaXML);
}
//________________________________________________________________________________________________________
function SubarbolXML_ImagenesDisponibles($cmd,$idperfilsoft)
{
	global $TbMsg;
	
	$cadenaXML="";
	$gidimagen=0;

	$cmd->texto="SELECT imagenes.* FROM imagenes
								INNER JOIN procedimientos ON procedimientos.idperfilsoft=imagenes.idperfilsoft
								WHERE procedimientos.idperfilsoft=".$idperfilsoft." 
								AND  imagenes.codpar>0
								ORDER by imagenes.descripcion";
	$rs=new Recordset; 
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($cadenaXML); // Error al abrir recordset
	if ($rs->numeroderegistros>0) {
		$cadenaXML.='<DISPONIBLESIMAGENES';
		$cadenaXML.=' imagenodo="../images/iconos/carpeta.gif"';
		$cadenaXML.=' infonodo="'.$TbMsg[4].'"';
		$cadenaXML.='>';
	}
	while (!$rs->EOF){
		if ($gidimagen!=$rs->campos["idperfilsoft"]){
			if ($gidimagen){
				$cadenaXML.='</IMAGENES>';
			}
			$gidimagen=$rs->campos["idperfilsoft"];
			$cadenaXML.='<IMAGENES';
			// Atributos
			$cadenaXML.=' imagenodo="../images/iconos/imagenes.gif"';
			$cadenaXML.=' infonodo="'.$rs->campos["descripcion"].'"';
			$cadenaXML.='>';
		}
		$rs->Siguiente();
	}
	if ($gidimagen){
		$cadenaXML.='</IMAGENES>';
		$cadenaXML.='</DISPONIBLESIMAGENES>';
	}
	$rs->Cerrar();
	return($cadenaXML);
}
?>
