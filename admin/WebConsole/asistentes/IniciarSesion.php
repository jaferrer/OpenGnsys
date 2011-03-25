<?
// *************************************************************************************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2009-2010
// Fecha Última modificación: Agosto-2010
// Nombre del fichero: IniciarSesion.php
// Descripción : 
//		Implementación� del comando "Iniciar Sesión"
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/constantes.php");
include_once("../includes/comunes.php");
include_once("../includes/CreaComando.php");
include_once("../includes/HTMLSELECT.php");
include_once("../includes/TomaDato.php");
include_once("../idiomas/php/".$idioma."/comandos/iniciarsesion_".$idioma.".php");
//________________________________________________________________________________________________________
include_once("./includes/capturaacciones.php");
//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion);
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexión con servidor B.D.
//___________________________________________________________________________________________________
?>
<HTML>
<TITLE>Administración web de aulas</TITLE>
<HEAD>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<LINK rel="stylesheet" type="text/css" href="../estilos.css">
<SCRIPT language="javascript" src="./jscripts/IniciarSesion.js"></SCRIPT>
<SCRIPT language="javascript" src="../clases/jscripts/HttpLib.js"></SCRIPT>
<? echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/comandos/iniciarsesion_'.$idioma.'.js"></SCRIPT>'?>
<? echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/comandos/comunescomandos_'.$idioma.'.js"></SCRIPT>'?>
<SCRIPT language="javascript" src="./jscripts/comunescomandos.js"></SCRIPT>
</HEAD>
<BODY>
<?
switch($ambito){
		case $AMBITO_CENTROS :
			$urlimg='../images/iconos/centros.gif';
			$textambito=$TbMsg[0];
			break;
		case $AMBITO_GRUPOSAULAS :
			$urlimg='../images/iconos/carpeta.gif';
			$textambito=$TbMsg[1];
			break;
		case $AMBITO_AULAS :
			$urlimg='../images/iconos/aula.gif';
			$textambito=$TbMsg[2];
			break;
		case $AMBITO_GRUPOSORDENADORES :
			$urlimg='../images/iconos/carpeta.gif';
			$textambito=$TbMsg[3];
			break;
		case $AMBITO_ORDENADORES :
			$urlimg='../images/iconos/ordenador.gif';
			$textambito=$TbMsg[4];
			break;
	}
	echo '<p align=center><span class=cabeceras>'.$TbMsg[5].'&nbsp;</span><br>';
	echo '<IMG src="'.$urlimg.'">&nbsp;&nbsp;<span align=center class=subcabeceras><U>'.$TbMsg[6].': '.$textambito.','.$nombreambito.'</U></span>&nbsp;&nbsp;</span></p>';
?>
	<P align=center>
	<SPAN align=center class=subcabeceras><? echo $TbMsg[7] ?></SPAN>
	</BR>
<form  align=center name="fdatos"> 
	<TABLE  id="tabla_conf" align=center border=0 cellPadding=1 cellSpacing=1 class=tabla_datos>
		<TR>
			<TH align=center>&nbsp;&nbsp;</TH>
			<TH align=center>&nbsp;<? echo $TbMsg[8] ?>&nbsp;</TH>
			<TH align=center>&nbsp;<? echo $TbMsg[9] ?>&nbsp;</TH>
		</TR>
			<?
				echo tabla_configuraciones($cmd,$idambito);
			?>
	</TABLE>
</FORM>
<?
	//________________________________________________________________________________________________________
	include_once("./includes/formularioacciones.php");
	//________________________________________________________________________________________________________
	include_once("./includes/opcionesacciones.php");
	//________________________________________________________________________________________________________
?>
</BODY>
</HTML>
<?
/**************************************************************************************************************************************************
	Recupera los datos de un ordenador
		Parametros: 
		- cmd: Una comando ya operativo (con conexiónabierta)  
		- ido: El identificador del ordenador
________________________________________________________________________________________________________*/
function toma_propiedades($cmd,$idordenador){
	global $nombreordenador;
	global $ip;
	global $mac;
	global $idperfilhard;
	global $idservidordhcp;
	global $idservidorrembo;
	$rs=new Recordset; 
	$cmd->texto="SELECT nombreordenador,ip,mac,idperfilhard FROM ordenadores WHERE idordenador='".$idordenador."'";
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(false); // Error al abrir recordset
	$rs->Primero(); 
	if (!$rs->EOF){
		$nombreordenador=$rs->campos["nombreordenador"];
		$ip=$rs->campos["ip"];
		$mac=$rs->campos["mac"];
		$idperfilhard=$rs->campos["idperfilhard"];
		$rs->Cerrar();
		return(true);
	}
	else
		return(false);
}
/*________________________________________________________________________________________________________
	Crea la tabla de configuraciones y perfiles a crear
________________________________________________________________________________________________________*/
function tabla_configuraciones($cmd,$idordenador){
	global $idcentro;
	$tablaHtml="";
	$cmd->texto="SELECT ordenadores_particiones.idnombreso,ordenadores_particiones.numpar,ordenadores_particiones.tamano,nombresos.nombreso,tipospar.tipopar,
				imagenes.descripcion as imagen,perfilessoft.descripcion as perfilsoft,sistemasficheros.descripcion as sistemafichero
				FROM ordenadores
				INNER JOIN ordenadores_particiones ON ordenadores_particiones.idordenador=ordenadores.idordenador
				LEFT OUTER JOIN nombresos ON nombresos.idnombreso=ordenadores_particiones.idnombreso
				INNER JOIN tipospar ON tipospar.codpar=ordenadores_particiones.codpar
				LEFT OUTER JOIN imagenes ON imagenes.idimagen=ordenadores_particiones.idimagen
				LEFT OUTER JOIN perfilessoft ON perfilessoft.idperfilsoft=ordenadores_particiones.idperfilsoft
				LEFT OUTER JOIN sistemasficheros ON sistemasficheros.idsistemafichero=ordenadores_particiones.idsistemafichero
				WHERE ordenadores.idordenador=".$idordenador." AND tipospar.clonable=1 ORDER BY ordenadores_particiones.numpar";
				
	$rs->Comando=&$cmd; 
	$rs=new Recordset; 
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return($tablaHtml); // Error al abrir recordset
	$rs->Primero(); 
	while (!$rs->EOF){
		if(!empty($rs->campos["idnombreso"])){
			$tablaHtml.='<TR>'.chr(13);
			$tablaHtml.='<TD ><input type="radio" name="particion"  value='.$rs->campos["numpar"].'></TD>'.chr(13);
			$tablaHtml.='<TD align=center>&nbsp;'.$rs->campos["numpar"].'&nbsp;</TD>'.chr(13);
			$tablaHtml.='<TD>&nbsp;'.$rs->campos["nombreso"].'&nbsp;</TD>'.chr(13);
			$tablaHtml.='</TR>'.chr(13);
		}
		$rs->Siguiente();
	}
	$rs->Cerrar();
	return($tablaHtml);
}
?>