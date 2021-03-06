<?php
// *************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2009-2010
// Fecha útima modificación: Marzo-2006
// Nombre del fichero: menubrowser.php
// Descripción : 
//		Muestra menu en el browser del cliente
// ****************************************************************************
$idioma="esp"; // Por defecto idoma español
include_once("../idiomas/php/".$idioma."/msgbrowser_".$idioma.".php");

$idx="";
$msg="";

if (isset($_GET["msg"])) $msg=$_GET["msg"];  // Recoge indice del mensaje
if (isset($_GET["idx"])) $idx=$_GET["idx"];  // Recoge indice del mensaje

if(!empty($msg))
	$mensaje=UrlDecode($msg);
else
	$mensaje=$TbMsg[$idx];
?>
<HTML>
<TITLE>Administración web de aulas</TITLE>
<HEAD>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
	<LINK rel="stylesheet" type="text/css" href="../estilos.css">
	<SCRIPT language="javascript">

	</SCRIPT>
	<STYLE TYPE="text/css">

	.deepsea, .deepsea TD, .deepsea TH
	{
	background-color:#026afe;
	color:#FFFFFF;
	font-family: sans-serif;
	font-weight:600; 
	}

	.tdbarra{
		background: url('../images/iconos/barraven.png');
		color:#FFFFFF;
		font-family: sans-serif;
		font-size: 12px;
		font-weight:300; 
		BORDER-BOTTOM: #000000 1px solid;
		BORDER-LEFT: #FFFFFF 1px solid;
		BORDER-RIGHT: #000000 1px solid;
		BORDER-TOP: #FFFFFF 1px solid;
	} 
	.tdclien{
		background: url('../images/iconos/clienven.png');
		color:#000000;
		font-family: sans-serif;
		font-size: 14px;
		font-weight:300; 
		BORDER-BOTTOM: #999999 1px solid;
		BORDER-LEFT: #FFFFFF 1px solid;
		BORDER-RIGHT: #999999 1px solid;
		BORDER-TOP: #FFFFFF 1px solid;
	} 
	</STYLE>
</HEAD>
<BODY>
<?php
if(empty($idx) && empty($msg)) {
	// No hay operaciones realizandose
	echo '<BR><BR><BR><BR><BR>';
	echo '<TABLE cellspacing=0 cellpadding=2 align=center border=0>';
	echo '<TR><TD align=center><IMG border=0 src="../images/iconos/logoopengnsys.png"></TD></TR>';
	echo '<TR><TD align=center><SPAN style="COLOR: #999999;FONT-FAMILY: Arial;FONT-SIZE: 12px;">Iniciando...</TD></TR>';
	echo '</TR>';
	echo '</TABLE>';
}
else{
	// Se está realizando una operacion
	echo '<h1>' . $TbMsg[23]   . ' </h1>';
	echo '<h1>' . $TbMsg[24]   . ' </h1>';
	echo '<BR><BR><BR>';
	echo '<TABLE  cellspacing=0 cellpadding=2 align=center border=0>';
	echo '<TR><TD align=center class="tdbarra">Opengnsys Browser Message</TD><TR>';
	echo '<TR><TD class="tdclien" valign=center >&nbsp;&nbsp;&nbsp;'.$mensaje.'&nbsp;&nbsp;&nbsp;</TD></TR>';
	echo '</TABLE>';
}
?>
</BODY>
</HTML>
