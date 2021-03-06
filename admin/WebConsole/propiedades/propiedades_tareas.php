<?php 
// *************************************************************************************************************************************************
// Aplicación WEB: ogAdmWebCon
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Año 2009-2010
// Fecha Última modificación: Agosto-2010
// Nombre del fichero: propiedades_tareas.php
// Descripción : 
//		 Presenta el formulario de captura de datos de una tarea para insertar,modificar y eliminar
// *************************************************************************************************************************************************
include_once("../includes/ctrlacc.php");
include_once("../includes/opciones.php");
include_once("../includes/CreaComando.php");
include_once("../clases/AdoPhp.php");
include_once("../includes/HTMLSELECT.php");
include_once("../includes/HTMLCTESELECT.php");
include_once("../includes/constantes.php");
include_once("../includes/comunes.php");
include_once("../includes/TomaDato.php");	
include_once("../idiomas/php/".$idioma."/propiedades_tareas_".$idioma.".php");
//________________________________________________________________________________________________________
$opcion=0;
$opciones=array($TbMsg[0],$TbMsg[1],$TbMsg[2],$TbMsg[3]);
//________________________________________________________________________________________________________
$idtarea=0; 
$descripcion="";
$comentarios="";
$grupoid=0;

if (isset($_GET["opcion"])) $opcion=$_GET["opcion"];  // Recoge parametros
if (isset($_GET["idtarea"])) $idtarea=$_GET["idtarea"]; 
if (isset($_GET["grupoid"])) $grupoid=$_GET["grupoid"]; 
if (isset($_GET["identificador"])) $idtarea=$_GET["identificador"];
//________________________________________________________________________________________________________
$cmd=CreaComando($cadenaconexion); // Crea objeto comando
if (!$cmd)
	Header('Location: '.$pagerror.'?herror=2'); // Error de conexión con servidor B.D.
if  ($opcion!=$op_alta){
	$resul=TomaPropiedades($cmd,$idtarea);
	if (!$resul)
		Header('Location: '.$pagerror.'?herror=3'); // Error de recuperación de datos.
}
//________________________________________________________________________________________________________
?>
<HTML>
<TITLE>Administración web de aulas</TITLE>
<HEAD>
	<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
	<LINK rel="stylesheet" type="text/css" href="../estilos.css">
	<SCRIPT language="javascript" src="../jscripts/propiedades_tareas.js"></SCRIPT>
	<SCRIPT language="javascript" src="../jscripts/opciones.js"></SCRIPT>
	<SCRIPT language="javascript" src="../clases/jscripts/HttpLib.js"></SCRIPT>			
	<?php echo '<SCRIPT language="javascript" src="../idiomas/javascripts/'.$idioma.'/propiedades_tareas_'.$idioma.'.js"></SCRIPT>'?>
</HEAD>
<BODY>
<DIV  align=center>
<FORM  name="fdatos" action="../gestores/gestor_tareas.php" method="post"> 
	<INPUT type=hidden name=opcion value=<?php echo $opcion?>>
	<INPUT type=hidden name=idtarea value=<?php echo $idtarea?>>
	<INPUT type=hidden name=grupoid value=<?php echo $grupoid?>>
	<INPUT type=hidden name=idambito value=<?php echo $idambito?>>
	<P align=center class=cabeceras><?php echo $TbMsg[4]?><BR>
	<SPAN align=center class=subcabeceras><?php echo $opciones[$opcion]?></SPAN></P>
	<TABLE  align=center border=0 cellPadding=1 cellSpacing=1 class=tabla_datos >
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
	<TR>
			<TH align=center>&nbsp;<?php echo $TbMsg[5]?>&nbsp;</TD>
			<?php if ($opcion==$op_eliminacion)
					echo '<TD style="width:300">'.$descripcion.'</TD>';
				else
					echo '<TD><INPUT  class="formulariodatos" name=descripcion style="width:300" type=text value="'.$descripcion.'"></TD>';?>
		</TR>
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
	<TR>
			<TH align=center>&nbsp;<?php echo $TbMsg[6]?>&nbsp;</TD>
			<?php if ($opcion==$op_eliminacion)
					echo '<TD>'.$comentarios.'</TD>';
				else
					echo '<TD><TEXTAREA   class="formulariodatos" name=comentarios rows=3 cols=55>'.$comentarios.'</TEXTAREA></TD>';
			?>
		</TR>	
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
		<TR>
			<TH align=center>&nbsp;<?php echo $TbMsg[7]?>&nbsp;</TD>
			<?php
				if ($opcion==$op_eliminacion){
					$Mambitos[$AMBITO_CENTROS]=$TbMsg[10];
					$Mambitos[$AMBITO_GRUPOSAULAS]=$TbMsg[11];
					$Mambitos[$AMBITO_AULAS]=$TbMsg[12];
					$Mambitos[$AMBITO_GRUPOSORDENADORES]=$TbMsg[13];
					$Mambitos[$AMBITO_ORDENADORES]=$TbMsg[14];
					echo '<TD>'.$Mambitos[$ambito].'</TD>';
				}
				else{
					$parametros=$AMBITO_CENTROS."=".$TbMsg[10].chr(13);
					$parametros.=$AMBITO_GRUPOSAULAS."=".$TbMsg[11].chr(13);
					$parametros.=$AMBITO_AULAS."=".$TbMsg[12].chr(13);
					$parametros.=$AMBITO_GRUPOSORDENADORES."=".$TbMsg[13].chr(13);
					$parametros.=$AMBITO_ORDENADORES."=".$TbMsg[14];
					echo '<TD>'.HTMLCTESELECT($parametros,"ambito","estilodesple"," ",$ambito,200,"chgdespleambito").'</TD>';
				}
			?>
		</TR>
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
		<TR>
			<TH align=center>&nbsp;<?php echo $TbMsg[8]?>&nbsp;</TD>
			<?php
				if ($opcion==$op_eliminacion){
					tomaDescriAmbito($cmd,$ambito,$idambito,$textambito);				
					echo '<TD>'.$textambito.'</TD>';
				}
				else{
					echo '<TD id="despleambito">'.tomaSelectAmbito($cmd,$ambito,$idambito,$idcentro,250).'</TD>';
				}
			?>
		</TR>		
<!------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
	</TABLE>
</FORM>
<?php
//________________________________________________________________________________________________________
include_once("../includes/opcionesbotonesop.php");
//________________________________________________________________________________________________________
?>
</BODY>
</HTML>
<?php
//________________________________________________________________________________________________________
//	Recupera los datos de una tarea
//		Parametros: 
//		- cmd: Una comando ya operativo (con conexión abierta)  
//		- id: El identificador de la tarea
//________________________________________________________________________________________________________
function TomaPropiedades($cmd,$id){
	global $descripcion;
	global $comentarios;
	global $ambito;
	global $idambito;
	$rs=new Recordset; 
	$cmd->texto="SELECT * FROM tareas WHERE idtarea=".$id;
	$rs->Comando=&$cmd; 
	if (!$rs->Abrir()) return(false); // Error al abrir recordset
	$rs->Primero(); 
	if (!$rs->EOF){
		$descripcion=$rs->campos["descripcion"];
		$comentarios=$rs->campos["comentarios"];
		$ambito=$rs->campos["ambito"];
		$idambito=$rs->campos["idambito"];
		$rs->Cerrar();
		return(true);
	}
	else
		return(false);
}
?>
