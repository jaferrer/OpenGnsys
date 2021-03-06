// *******************************************************************************************************
// Servicio: ogAdmServer
// Autor: José Manuel Alonso (E.T.S.I.I.) Universidad de Sevilla
// Fecha Creación: Marzo-2010
// Fecha Última modificación: Marzo-2010
// Nombre del fichero: ogAdmServer.cpp
// Descripción :Este fichero implementa el servicio de administración general del sistema
// *******************************************************************************************************
#include "ogAdmServer.h"
#include "ogAdmLib.c"
//________________________________________________________________________________________________________
//	Función: tomaConfiguracion
//
//	Descripción:
//		Lee el fichero de configuración del servicio
//	Parámetros:
//		filecfg : Ruta completa al fichero de configuración
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error 
//________________________________________________________________________________________________________
BOOLEAN tomaConfiguracion(char* filecfg) {
	char modulo[] = "tomaConfiguracion()";

	if (filecfg == NULL || strlen(filecfg) == 0) {
		errorLog(modulo, 1, FALSE); // Fichero de configuración del servicio vacío
		return (FALSE);
	}
	FILE *fcfg;
	long lSize;
	char * buffer, *lineas[MAXPRM], *dualparametro[2];
	int i, numlin, resul;

	fcfg = fopen(filecfg, "rt");
	if (fcfg == NULL) {
		errorLog(modulo, 2, FALSE); // No existe fichero de configuración del servicio
		return (FALSE);
	}

	fseek(fcfg, 0, SEEK_END);
	lSize = ftell(fcfg); // Obtiene tamaño del fichero.
	rewind(fcfg);
	buffer = (char*) reservaMemoria(lSize + 1); // Toma memoria para el buffer de lectura.
	if (buffer == NULL) { // No hay memoria suficiente para el buffer
		errorLog(modulo, 3, FALSE);
		return (FALSE);
	}
	fread(buffer, 1, lSize, fcfg); // Lee contenido del fichero
	buffer[lSize] = (char) NULL;
	fclose(fcfg);

	servidoradm[0] = (char) NULL; //inicializar variables globales
	usuario[0] = (char) NULL;
	pasguor[0] = (char) NULL;
	datasource[0] = (char) NULL;
	catalog[0] = (char) NULL;
	aulaup[0] = (char) NULL;

	numlin = splitCadena(lineas, buffer, '\n');
	for (i = 0; i < numlin; i++) {
		splitCadena(dualparametro, lineas[i], '=');
		resul = strcmp(StrToUpper(dualparametro[0]), "SERVIDORADM");
		if (resul == 0)
			strcpy(servidoradm, dualparametro[1]);
		resul = strcmp(StrToUpper(dualparametro[0]), "PUERTO");
		if (resul == 0)
			strcpy(puerto, dualparametro[1]);
		resul = strcmp(StrToUpper(dualparametro[0]), "USUARIO");
		if (resul == 0)
			strcpy(usuario, dualparametro[1]);
		resul = strcmp(StrToUpper(dualparametro[0]), "PASSWORD");
		if (resul == 0)
			strcpy(pasguor, dualparametro[1]);
		resul = strcmp(StrToUpper(dualparametro[0]), "DATASOURCE");
		if (resul == 0)
			strcpy(datasource, dualparametro[1]);
		resul = strcmp(StrToUpper(dualparametro[0]), "CATALOG");
		if (resul == 0)
			strcpy(catalog, dualparametro[1]);
		resul = strcmp(StrToUpper(dualparametro[0]), "AULAUP");
		if (resul == 0)
			strcpy(catalog, dualparametro[1]);
	}
	if (servidoradm[0] == (char) NULL) {
		liberaMemoria(buffer);
		errorLog(modulo, 4, FALSE); // Falta parámetro SERVIDORADM
		return (FALSE);
	}
	if (puerto[0] == (char) NULL) {
		liberaMemoria(buffer);
		errorLog(modulo, 5, FALSE); // Falta parámetro PUERTO
		return (FALSE);
	}
	if (usuario[0] == (char) NULL) {
		liberaMemoria(buffer);
		errorLog(modulo, 6, FALSE); // Falta parámetro USUARIO
		return (FALSE);
	}
	if (pasguor[0] == (char) NULL) {
		liberaMemoria(buffer);
		errorLog(modulo, 7, FALSE); // Falta parámetro PASSWORD
		return (FALSE);
	}
	if (datasource[0] == (char) NULL) {
		liberaMemoria(buffer);
		errorLog(modulo, 8, FALSE); // Falta parámetro DATASOURCE
		return (FALSE);
	}
	if (catalog[0] == (char) NULL) {
		liberaMemoria(buffer);
		errorLog(modulo, 9, FALSE); // Falta parámetro CATALOG
		return (FALSE);
	}
	if (aulaup[0] == (char) NULL) {
		strcpy(aulaup, "0"); // Por defecto el conmutador de registro automático esta en off

	}
	liberaMemoria(buffer);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: gestionaTrama
//
//		Descripción:
//			Procesa las tramas recibidas .
//		Parametros:
//			- s : Socket usado para comunicaciones
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN gestionaTrama(SOCKET *socket_c)
{
	TRAMA* ptrTrama;
	int i, res;
	char *nfn;
	char modulo[] = "gestionaTrama()";

	ptrTrama=recibeTrama(socket_c);
	
	if (ptrTrama){
		INTROaFINCAD(ptrTrama);
		nfn = copiaParametro("nfn",ptrTrama); // Toma nombre de la función

		for (i = 0; i < MAXIMAS_FUNCIONES; i++) { // Recorre funciones que procesan las tramas
			res = strcmp(tbfuncionesServer[i].nf, nfn);
			if (res == 0) { // Encontrada la función que procesa el mensaje
				liberaMemoria(nfn);
				res=tbfuncionesServer[i].fptr(socket_c, ptrTrama); // Invoca la función
				liberaMemoria(ptrTrama->parametros);
				liberaMemoria(ptrTrama);
				return(res);
			}
		}
		liberaMemoria(nfn);
		liberaMemoria(ptrTrama->parametros);
		liberaMemoria(ptrTrama);
		/*
		 Sólo puede ser un comando personalizado o su notificación
		if (ptrTrama->tipo == MSG_COMANDO)
			return (Comando(socket_c, ptrTrama));
		else {
			if (ptrTrama->tipo == MSG_NOTIFICACION)
				return (RESPUESTA_Comando(socket_c, ptrTrama));
			else
				errorLog(modulo, 18, FALSE); // No se reconoce el mensaje
		}
		*/
	}
	else
		errorLog(modulo, 17, FALSE); // Error en la recepción
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Sondeo
//
//	Descripción:
//		Solicita a los clientes su disponibiliad para recibir comandos interactivos
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros del mensaje
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Sondeo(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "Sondeo()";

	if (!enviaComando(ptrTrama, CLIENTE_APAGADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: respuestaSondeo
//
//	Descripción:
//		Recupera el estatus de los ordenadores solicitados leyendo la tabla de sockets
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros del mensaje
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN respuestaSondeo(SOCKET *socket_c, TRAMA* ptrTrama) {
	int i;
	long lSize;
	char *iph, *Ipes;
	char modulo[] = "respuestaSondeo()";

	iph = copiaParametro("iph",ptrTrama); // Toma dirección/es IP
	lSize = strlen(iph); // Calcula longitud de la cadena de direccion/es IPE/S
	Ipes = (char*) reservaMemoria(lSize + 1);
	if (Ipes == NULL) {
		liberaMemoria(iph);
		errorLog(modulo, 3, FALSE);
		return (FALSE);
	}
	strcpy(Ipes, iph); // Copia cadena de IPES
	liberaMemoria(iph);
	initParametros(ptrTrama,0);
	strcpy(ptrTrama->parametros, "tso="); // Compone retorno tso (sistemas operativos de los clientes )
	for (i = 0; i < MAXIMOS_CLIENTES; i++) {
		if (strncmp(tbsockets[i].ip, "\0", 1) != 0) { // Si es un cliente activo
			if (contieneIP(Ipes, tbsockets[i].ip)) { // Si existe la IP en la cadena
				strcat(ptrTrama->parametros, tbsockets[i].ip); // Compone retorno
				strcat(ptrTrama->parametros, "/"); // "ip/sistema operativo;"
				strcat(ptrTrama->parametros, tbsockets[i].estado);
				strcat(ptrTrama->parametros, ";");
			}
		}
	}
	strcat(ptrTrama->parametros, "\r");
	liberaMemoria(Ipes);
	if (!mandaTrama(socket_c, ptrTrama)) {
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Actualizar
//
//	Descripción:
//		Obliga a los clientes a iniciar sesión en el sistema
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros del mensaje
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Actualizar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "Actualizar()";

	if (!enviaComando(ptrTrama, CLIENTE_APAGADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Purgar
//
//	Descripción:
//		Detiene la ejecución del browser en el cliente
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros del mensaje
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Purgar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "Purgar()";

	if (!enviaComando(ptrTrama, CLIENTE_APAGADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: ConsolaRemota
//
//	Descripción:
// 		Envia un script al cliente, éste lo ejecuta y manda el archivo que genera la salida por pantalla
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros del mensaje
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN ConsolaRemota(SOCKET *socket_c, TRAMA* ptrTrama)
{
	char *iph,fileco[LONPRM],msglog[LONSTD],*ptrIpes[MAXIMOS_CLIENTES];;
	FILE* f;
	int i,lon;
	char modulo[] = "ConsolaRemota()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	INTROaFINCAD(ptrTrama);
	/* Destruye contenido del fichero de eco anterior */
	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip del cliente
	lon = splitCadena(ptrIpes,iph,';');
	for (i = 0; i < lon; i++) {
		sprintf(fileco,"/tmp/_Seconsola_%s",ptrIpes[i]); // Nombre que tendra el archivo en el Servidor
		f = fopen(fileco, "wt");
		fclose(f);
	}
	liberaMemoria(iph);
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: EcoConsola
//
//	Descripción:
//		Solicita el eco de una consola remota almacenado en un archivo de eco
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros del mensaje
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN EcoConsola(SOCKET *socket_c, TRAMA* ptrTrama)
{
	char *iph,fileco[LONPRM],*buffer;
	int lSize;
	char modulo[] = "EcoConsola()";

	INTROaFINCAD(ptrTrama);
	// Lee archivo de eco de consola
	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip del cliente
	sprintf(fileco,"/tmp/_Seconsola_%s",iph); // Nombre del archivo en el Servidor
	liberaMemoria(iph);
	lSize=lonArchivo(fileco);
	if(lSize>0){ // Si el fichero tiene contenido...
		initParametros(ptrTrama,lSize+LONGITUD_PARAMETROS);
		buffer=leeArchivo(fileco);
		sprintf(ptrTrama->parametros,"res=%s\r",buffer);
		liberaMemoria(buffer);
	}
	else{
		initParametros(ptrTrama,0);
		sprintf(ptrTrama->parametros,"res=\r");
	}
	ptrTrama->tipo=MSG_RESPUESTA; // Tipo de mensaje
	if (!mandaTrama(socket_c, ptrTrama)) {
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: clienteDisponible
//
//	Descripción:
//		Comprueba la disponibilidad del cliente para recibir comandos interactivos
//	Parametros:
//		- ip : La ip del cliente a buscar
//		- idx: (Salida)  Indice que ocupa el cliente, de estar ya registrado
//	Devuelve:
//		TRUE: Si el cliente está disponible
//		FALSE: En caso contrario
// ________________________________________________________________________________________________________
BOOLEAN clienteDisponible(char *ip, int* idx) {
	int estado;

	if (clienteExistente(ip, idx)) {
		estado = strcmp(tbsockets[*idx].estado, CLIENTE_OCUPADO); // Cliente ocupado
		if (estado == 0)
			return (FALSE);

		estado = strcmp(tbsockets[*idx].estado, CLIENTE_APAGADO); // Cliente apagado
		if (estado == 0)
			return (FALSE);

		estado = strcmp(tbsockets[*idx].estado, CLIENTE_INICIANDO); // Cliente en proceso de inclusión
		if (estado == 0)
			return (FALSE);

		return (TRUE); // En caso contrario el cliente está disponible
	}
	return (FALSE); // Cliente no está registrado en el sistema
}
// ________________________________________________________________________________________________________
// Función: clienteExistente
//
//	Descripción:
//		Comprueba si el cliente está registrado en la tabla de socket del sistema
//	Parametros:
//		- ip : La ip del cliente a buscar
//		- idx:(Salida)  Indice que ocupa el cliente, de estar ya registrado
//	Devuelve:
//		TRUE: Si el cliente está registrado
//		FALSE: En caso contrario
// ________________________________________________________________________________________________________
BOOLEAN clienteExistente(char *ip, int* idx) {
	int i;
	for (i = 0; i < MAXIMOS_CLIENTES; i++) {
		if (contieneIP(ip, tbsockets[i].ip)) { // Si existe la IP en la cadena
			*idx = i;
			return (TRUE);
		}
	}
	return (FALSE);
}
// ________________________________________________________________________________________________________
// Función: hayHueco
// 
// 	Descripción:
// 		Esta función devuelve TRUE o FALSE dependiendo de que haya hueco en la tabla de sockets para un nuevo cliente.
// 	Parametros:
// 		- idx:   Primer indice libre que se podrn utilizar
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN hayHueco(int *idx) {
	int i;

	for (i = 0; i < MAXIMOS_CLIENTES; i++) {
		if (strncmp(tbsockets[i].ip, "\0", 1) == 0) { // Hay un hueco
			*idx = i;
			return (TRUE);
		}
	}
	return (FALSE);
}
// ________________________________________________________________________________________________________
// Función: InclusionClienteWin
//
//	Descripción:
//		Esta función incorpora el socket de un nuevo cliente Windows o Linux a la tabla de clientes 
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN InclusionClienteWinLnx(SOCKET *socket_c, TRAMA *ptrTrama)
 {
	char modulo[] = "InclusionClienteWinLnx()";
	int res,idordenador,lon;
	char nombreordenador[LONFIL];
		
	res=procesoInclusionClienteWinLnx(socket_c, ptrTrama,&idordenador,nombreordenador);
	
	// Prepara la trama de respuesta

	initParametros(ptrTrama,0);
	ptrTrama->tipo=MSG_RESPUESTA;
	lon = sprintf(ptrTrama->parametros, "nfn=RESPUESTA_InclusionClienteWinLnx\r");
	lon += sprintf(ptrTrama->parametros + lon, "ido=%d\r", idordenador);
	lon += sprintf(ptrTrama->parametros + lon, "npc=%s\r", nombreordenador);	
	lon += sprintf(ptrTrama->parametros + lon, "res=%d\r", res);	
	
	if (!mandaTrama(socket_c, ptrTrama)) {
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	return (TRUE);	
}
// ________________________________________________________________________________________________________
// Función: procesoInclusionClienteWinLnx
//
//	Descripción:
//		Implementa el proceso de inclusión en el sistema del Cliente Windows o Linux
//	Parámetros de entrada:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Parámetros de salida:
//		- ido: Identificador del ordenador
//		- nombreordenador: Nombre del ordenador
//	Devuelve:
//		Código del error producido en caso de ocurrir algún error, 0 si el proceso es correcto
// ________________________________________________________________________________________________________
BOOLEAN procesoInclusionClienteWinLnx(SOCKET *socket_c, TRAMA *ptrTrama,int *idordenador,char* nombreordenador)
 {
	char msglog[LONSTD], sqlstr[LONSQL];
	Database db;
	Table tbl;

	char *iph;
	char modulo[] = "procesoInclusionClienteWinLnx()";
	
	// Toma parámetros
	iph = copiaParametro("iph",ptrTrama); // Toma ip

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexión con la BD
		liberaMemoria(iph);
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (20);
	}

	// Recupera los datos del cliente
	sprintf(sqlstr,
			"SELECT idordenador,nombreordenador FROM ordenadores "
				" WHERE ordenadores.ip = '%s'", iph);

	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		liberaMemoria(iph);
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		db.Close();
		return (21);
	}

	if (tbl.ISEOF()) { // Si no existe el cliente
		liberaMemoria(iph);
		errorLog(modulo, 22, FALSE);
		db.liberaResult(tbl);
		db.Close();
		return (22);
	}

	if (ndebug == DEBUG_ALTO) {
		sprintf(msglog, "%s IP:%s", tbMensajes[2], iph);
		infoDebug(msglog);
	}
	if (!tbl.Get("idordenador", *idordenador)) {
		liberaMemoria(iph);
		db.liberaResult(tbl);
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		db.Close();
		return (FALSE);
	}
	if (!tbl.Get("nombreordenador", nombreordenador)) {
		liberaMemoria(iph);
		db.liberaResult(tbl);
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		db.Close();
		return (FALSE);
	}
	db.liberaResult(tbl);
	db.Close();
	
	if (!registraCliente(iph)) { // Incluyendo al cliente en la tabla de sokets
		liberaMemoria(iph);
		errorLog(modulo, 25, FALSE);
		return (25);
	}
	liberaMemoria(iph);
	return(0);
}
// ________________________________________________________________________________________________________
// Función: InclusionCliente
//
//	Descripción:
//		Esta función incorpora el socket de un nuevo cliente a la tabla de clientes y le devuelve alguna de sus propiedades:
//		nombre, identificador, tamaño de la caché , etc ...
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN InclusionCliente(SOCKET *socket_c, TRAMA *ptrTrama) {
	char modulo[] = "InclusionCliente()";

	if (!procesoInclusionCliente(socket_c, ptrTrama)) { // Ha habido algún error...
		initParametros(ptrTrama,0);
		strcpy(ptrTrama->parametros, "nfn=RESPUESTA_InclusionCliente\rres=0\r");
		if (!mandaTrama(socket_c, ptrTrama)) {
			errorLog(modulo, 26, FALSE);
			return (FALSE);
		}
	}
	return (TRUE);
}	
// ________________________________________________________________________________________________________
// Función: procesoInclusionCliente
//
//	Descripción:
//		Implementa el proceso de inclusión en el sistema del Cliente
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN procesoInclusionCliente(SOCKET *socket_c, TRAMA *ptrTrama) {
	char msglog[LONSTD], sqlstr[LONSQL];
	Database db;
	Table tbl;

	char *iph, *cfg;
	char nombreordenador[LONFIL];
	int lon, resul, idordenador, idmenu, cache, idproautoexec, idaula, idcentro;
	char modulo[] = "procesoInclusionCliente()";

	// Toma parámetros
	iph = copiaParametro("iph",ptrTrama); // Toma ip
	cfg = copiaParametro("cfg",ptrTrama); // Toma configuracion

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexión con la BD
		liberaMemoria(iph);
		liberaMemoria(cfg);
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	// Recupera los datos del cliente
	sprintf(sqlstr,
			"SELECT ordenadores.*,aulas.idaula,centros.idcentro FROM ordenadores "
				" INNER JOIN aulas ON aulas.idaula=ordenadores.idaula"
				" INNER JOIN centros ON centros.idcentro=aulas.idcentro"
				" WHERE ordenadores.ip = '%s'", iph);

	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	if (tbl.ISEOF()) { // Si no existe el cliente
		errorLog(modulo, 22, FALSE);
		return (FALSE);
	}

	if (ndebug == DEBUG_ALTO) {
		sprintf(msglog, "%s IP:%s", tbMensajes[2], iph);
		infoDebug(msglog);
	}
	if (!tbl.Get("idordenador", idordenador)) {
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("nombreordenador", nombreordenador)) {
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idmenu", idmenu)) {
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("cache", cache)) {
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idproautoexec", idproautoexec)) {
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idaula", idaula)) {
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idcentro", idcentro)) {
		tbl.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	resul = actualizaConfiguracion(db, tbl, cfg, idordenador); // Actualiza la configuración del ordenador
	liberaMemoria(cfg);
	db.Close();

	if (!resul) {
		liberaMemoria(iph);
		errorLog(modulo, 29, FALSE);
		return (FALSE);
	}

	if (!registraCliente(iph)) { // Incluyendo al cliente en la tabla de sokets
		liberaMemoria(iph);
		errorLog(modulo, 25, FALSE);
		return (FALSE);
	}

	/*------------------------------------------------------------------------------------------------------------------------------
	 Prepara la trama de respuesta
	 -------------------------------------------------------------------------------------------------------------------------------*/
	initParametros(ptrTrama,0);
	ptrTrama->tipo=MSG_RESPUESTA;
	lon = sprintf(ptrTrama->parametros, "nfn=RESPUESTA_InclusionCliente\r");
	lon += sprintf(ptrTrama->parametros + lon, "ido=%d\r", idordenador);
	lon += sprintf(ptrTrama->parametros + lon, "npc=%s\r", nombreordenador);
	lon += sprintf(ptrTrama->parametros + lon, "che=%d\r", cache);
	lon += sprintf(ptrTrama->parametros + lon, "exe=%d\r", idproautoexec);
	lon += sprintf(ptrTrama->parametros + lon, "ida=%d\r", idaula);
	lon += sprintf(ptrTrama->parametros + lon, "idc=%d\r", idcentro);
	lon += sprintf(ptrTrama->parametros + lon, "res=%d\r", 1); // Confirmación proceso correcto

	if (!mandaTrama(socket_c, ptrTrama)) {
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	liberaMemoria(iph);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: actualizaConfiguracion
//
//	Descripción:
//		Esta función actualiza la base de datos con la configuracion de particiones de un cliente
//	Parámetros:
//		- db: Objeto base de datos (ya operativo)
//		- tbl: Objeto tabla
//		- cfg: cadena con una Configuración
//		- ido: Identificador del ordenador cliente
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
//	Especificaciones:
//		Los parametros de la configuración son:
//			par= Número de partición
//			cpt= Codigo o tipo de partición
//			sfi= Sistema de ficheros que está implementado en la partición
//			soi= Nombre del sistema de ficheros instalado en la partición
//			tam= Tamaño de la partición
// ________________________________________________________________________________________________________
BOOLEAN actualizaConfiguracion(Database db, Table tbl, char* cfg, int ido)
{
	char msglog[LONSTD], sqlstr[LONSQL];
	int lon, p, c,i, dato, swu, idsoi, idsfi,k;
	char *ptrPar[MAXPAR], *ptrCfg[6], *ptrDual[2], tbPar[LONSTD];
	char *ser, *disk, *par, *cpt, *sfi, *soi, *tam, *uso; // Parametros de configuración.
	char modulo[] = "actualizaConfiguracion()";

	lon = 0;
	p = splitCadena(ptrPar, cfg, '\n');
	for (i = 0; i < p; i++) {
		c = splitCadena(ptrCfg, ptrPar[i], '\t');

		// Si la 1ª línea solo incluye el número de serie del equipo; actualizar BD.
		if (i == 0 && c == 1) {
			splitCadena(ptrDual, ptrCfg[0], '=');
			ser = ptrDual[1];
			if (strlen(ser) > 0) {
				// Solo actualizar si número de serie no existía.
				sprintf(sqlstr, "UPDATE ordenadores SET numserie='%s'"
						" WHERE idordenador=%d AND numserie IS NULL",
						ser, ido);
				if (!db.Execute(sqlstr, tbl)) { // Error al insertar
					db.GetErrorErrStr(msglog);
					errorInfo(modulo, msglog);
					return (FALSE);
				}
			}
			continue;
		}

		// Distribución de particionado.
		disk = par = cpt = sfi = soi = tam = uso = NULL;

		splitCadena(ptrDual, ptrCfg[0], '=');
		disk = ptrDual[1]; // Número de disco

		splitCadena(ptrDual, ptrCfg[1], '=');
		par = ptrDual[1]; // Número de partición

		k=splitCadena(ptrDual, ptrCfg[2], '=');
		if(k==2){
			cpt = ptrDual[1]; // Código de partición
		}else{
			cpt = (char*)"0";
		}

		k=splitCadena(ptrDual, ptrCfg[3], '=');
		if(k==2){
			sfi = ptrDual[1]; // Sistema de ficheros
			/* Comprueba existencia del s0xistema de ficheros instalado */
			idsfi = checkDato(db, tbl, sfi, "sistemasficheros", "descripcion","idsistemafichero");
		}
		else
			idsfi=0;

		k=splitCadena(ptrDual, ptrCfg[4], '=');
		if(k==2){ // Sistema operativo detecdtado
			soi = ptrDual[1]; // Nombre del S.O. instalado
			/* Comprueba existencia del sistema operativo instalado */
			idsoi = checkDato(db, tbl, soi, "nombresos", "nombreso", "idnombreso");
		}
		else
			idsoi=0;

		splitCadena(ptrDual, ptrCfg[5], '=');
		tam = ptrDual[1]; // Tamaño de la partición

		splitCadena(ptrDual, ptrCfg[6], '=');
		uso = ptrDual[1]; // Porcentaje de uso del S.F.

		lon += sprintf(tbPar + lon, "(%s, %s),", disk, par);

		sprintf(sqlstr, "SELECT numdisk, numpar, codpar, tamano, uso, idsistemafichero, idnombreso"
				"  FROM ordenadores_particiones"
				" WHERE idordenador=%d AND numdisk=%s AND numpar=%s",
				ido, disk, par);


		if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
			errorLog(modulo, 21, FALSE);
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if (tbl.ISEOF()) { // Si no existe el registro
			sprintf(sqlstr, "INSERT INTO ordenadores_particiones(idordenador,numdisk,numpar,codpar,tamano,uso,idsistemafichero,idnombreso,idimagen)"
					" VALUES(%d,%s,%s,0x%s,%s,%s,%d,%d,0)",
					ido, disk, par, cpt, tam, uso, idsfi, idsoi);


			if (!db.Execute(sqlstr, tbl)) { // Error al insertar
				db.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				return (FALSE);
			}
		} else { // Existe el registro
			swu = TRUE; // Se supone que algún dato ha cambiado
			if (!tbl.Get("codpar", dato)) { // Toma dato
				tbl.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (FALSE);
			}
			if (strtol(cpt, NULL, 16) == dato) {// Parámetro tipo de partición (hexadecimal) igual al almacenado (decimal)
				if (!tbl.Get("tamano", dato)) { // Toma dato
					tbl.GetErrorErrStr(msglog); // Error al acceder al registro
					errorInfo(modulo, msglog);
					return (FALSE);
				}
				if (atoi(tam) == dato) {// Parámetro tamaño igual al almacenado
					if (!tbl.Get("idsistemafichero", dato)) { // Toma dato
						tbl.GetErrorErrStr(msglog); // Error al acceder al registro
						errorInfo(modulo, msglog);
						return (FALSE);
					}
					if (idsfi == dato) {// Parámetro sistema de fichero igual al almacenado
						if (!tbl.Get("idnombreso", dato)) { // Toma dato
							tbl.GetErrorErrStr(msglog); // Error al acceder al registro
							errorInfo(modulo, msglog);
							return (FALSE);
						}
						if (idsoi == dato) {// Parámetro sistema de fichero distinto al almacenado
							swu = FALSE; // Todos los parámetros de la partición son iguales, no se actualiza
						}
					}
				}
			}
			if (swu) { // Hay que actualizar los parámetros de la partición
				sprintf(sqlstr, "UPDATE ordenadores_particiones SET "
					" codpar=0x%s,"
					" tamano=%s,"
					" uso=%s,"
					" idsistemafichero=%d,"
					" idnombreso=%d,"
					" idimagen=0,"
					" idperfilsoft=0,"
					" fechadespliegue=NULL"
					" WHERE idordenador=%d AND numdisk=%s AND numpar=%s",
					cpt, tam, uso, idsfi, idsoi, ido, disk, par);
			} else {  // Actualizar porcentaje de uso.
				sprintf(sqlstr, "UPDATE ordenadores_particiones SET "
					" uso=%s"
					" WHERE idordenador=%d AND numdisk=%s AND numpar=%s",
					uso, ido, disk, par);
			}
			if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
				errorLog(modulo, 21, FALSE);
				db.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				return (FALSE);
			}
		}
	}
	lon += sprintf(tbPar + lon, "(0,0)");
	// Eliminar particiones almacenadas que ya no existen
	sprintf(sqlstr, "DELETE FROM ordenadores_particiones WHERE idordenador=%d AND (numdisk, numpar) NOT IN (%s)",
			ido, tbPar);
	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: checkDato
//
//	Descripción:
//		 Esta función comprueba si existe un dato en una tabla y si no es así lo incluye. devuelve en
//		cualquier caso el identificador del registro existenet o del insertado
//	Parámetros:
//		- db: Objeto base de datos (ya operativo)
//		- tbl: Objeto tabla
//		- dato: Dato
//		- tabla: Nombre de la tabla
//		- nomdato: Nombre del dato en la tabla
//		- nomidentificador: Nombre del identificador en la tabla
//	Devuelve:
//		El identificador del registro existente o el del insertado
//
//	Especificaciones:
//		En caso de producirse algún error se devuelve el valor 0
// ________________________________________________________________________________________________________

int checkDato(Database db, Table tbl, char *dato, const char*tabla,
		const char* nomdato, const char *nomidentificador) {
	char msglog[LONSTD], sqlstr[LONSQL];
	char modulo[] = "checkDato()";
	int identificador;

	if (strlen(dato) == 0)
		return (0); // EL dato no tiene valor
	sprintf(sqlstr, "SELECT %s FROM %s WHERE %s ='%s'", nomidentificador,
			tabla, nomdato, dato);

	// Ejecuta consulta
	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (0);
	}
	if (tbl.ISEOF()) { //  Software NO existente
		sprintf(sqlstr, "INSERT INTO %s (%s) VALUES('%s')", tabla, nomdato, dato);
		if (!db.Execute(sqlstr, tbl)) { // Error al insertar
			db.GetErrorErrStr(msglog); // Error al acceder al registro
			errorInfo(modulo, msglog);
			return (0);
		}
		// Recupera el identificador del software
		sprintf(sqlstr, "SELECT LAST_INSERT_ID() as identificador");
		if (!db.Execute(sqlstr, tbl)) { // Error al leer
			db.GetErrorErrStr(msglog); // Error al acceder al registro
			errorInfo(modulo, msglog);
			return (0);
		}
		if (!tbl.ISEOF()) { // Si existe registro
			if (!tbl.Get("identificador", identificador)) {
				tbl.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (0);
			}
		}
	} else {
		if (!tbl.Get(nomidentificador, identificador)) { // Toma dato
			tbl.GetErrorErrStr(msglog); // Error al acceder al registro
			errorInfo(modulo, msglog);
			return (0);
		}
	}
	return (identificador);
}
// ________________________________________________________________________________________________________
// Función: registraCliente
//
//	Descripción:
//		 Incluye al cliente en la tabla de sokets
//	Parámetros:
//		- iph: Dirección ip del cliente
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN registraCliente(char *iph) {
	int idx;

	if (!clienteExistente(iph, &idx)) { // Si no existe la IP ...
		if (!hayHueco(&idx)) { // Busca hueco para el nuevo cliente
			return (FALSE); // No hay huecos
		}
	}
	strcpy(tbsockets[idx].ip, iph); // Copia IP
	strcpy(tbsockets[idx].estado, CLIENTE_INICIANDO); // Actualiza el estado del cliente
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: AutoexecCliente
//
//	Descripción:
//		Envía archivo de autoexec al cliente
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN AutoexecCliente(SOCKET *socket_c, TRAMA *ptrTrama) {
	int lon;
	char *iph, *exe, msglog[LONSTD];
	Database db;
	FILE *fileexe;
	char fileautoexec[LONPRM];
	char parametros[LONGITUD_PARAMETROS];
	char modulo[] = "AutoexecCliente()";

	iph = copiaParametro("iph",ptrTrama); // Toma dirección IP del cliente
	exe = copiaParametro("exe",ptrTrama); // Toma identificador del procedimiento inicial

	sprintf(fileautoexec, "/tmp/Sautoexec-%s", iph);
	liberaMemoria(iph);
	fileexe = fopen(fileautoexec, "wb"); // Abre fichero de script
	if (fileexe == NULL) {
		errorLog(modulo, 52, FALSE);
		return (FALSE);
	}

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexión con la BD
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	initParametros(ptrTrama,0);
	if (recorreProcedimientos(db, parametros, fileexe, exe)) {
		lon = sprintf(ptrTrama->parametros, "nfn=RESPUESTA_AutoexecCliente\r");
		lon += sprintf(ptrTrama->parametros + lon, "nfl=%s\r", fileautoexec);
		lon += sprintf(ptrTrama->parametros + lon, "res=1\r");
	} else {
		lon = sprintf(ptrTrama->parametros, "nfn=RESPUESTA_AutoexecCliente\r");
		lon += sprintf(ptrTrama->parametros + lon, "res=0\r");
	}

	db.Close();
	fclose(fileexe);

	if (!mandaTrama(socket_c, ptrTrama)) {
		liberaMemoria(exe);
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	liberaMemoria(exe);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: recorreProcedimientos
//
//	Descripción:
//		Crea un archivo con el código de un procedimiento separando cada comando  por un salto de linea
//	Parámetros:
//		Database db,char* parametros,FILE* fileexe,char* idp
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN recorreProcedimientos(Database db, char* parametros, FILE* fileexe,
		char* idp) {
	int procedimientoid, lsize;
	char idprocedimiento[LONPRM], msglog[LONSTD], sqlstr[LONSQL];
	Table tbl;
	char modulo[] = "recorreProcedimientos()";

	/* Busca procedimiento */
	sprintf(sqlstr,
			"SELECT procedimientoid,parametros FROM procedimientos_acciones"
				" WHERE idprocedimiento=%s ORDER BY orden", idp);
	// Ejecuta consulta
	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	while (!tbl.ISEOF()) { // Recorre procedimientos
		if (!tbl.Get("procedimientoid", procedimientoid)) { // Toma dato
			tbl.GetErrorErrStr(msglog); // Error al acceder al registro
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if (procedimientoid > 0) { // Procedimiento recursivo
			sprintf(idprocedimiento, "%d", procedimientoid);
			if (!recorreProcedimientos(db, parametros, fileexe, idprocedimiento)) {
				return (FALSE);
			}
		} else {
			if (!tbl.Get("parametros", parametros)) { // Toma dato
				tbl.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (FALSE);
			}
			strcat(parametros, "@");
			lsize = strlen(parametros);
			fwrite(parametros, 1, lsize, fileexe); // Escribe el código a ejecutar
		}
		tbl.MoveNext();
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: ComandosPendientes
//
//	Descripción:
//		Esta función busca en la base de datos,comandos pendientes de ejecutar por un  ordenador  concreto
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN ComandosPendientes(SOCKET *socket_c, TRAMA *ptrTrama) 
{
	char *ido,*iph,pids[LONPRM];
	int ids, idx;
	char modulo[] = "ComandosPendientes()";

	iph = copiaParametro("iph",ptrTrama); // Toma dirección IP
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!clienteExistente(iph, &idx)) { // Busca índice del cliente
		liberaMemoria(iph);
		liberaMemoria(ido);
		errorLog(modulo, 47, FALSE);
		return (FALSE);
	}
	if (buscaComandos(ido, ptrTrama, &ids)) { // Existen comandos pendientes
		ptrTrama->tipo = MSG_COMANDO;
		sprintf(pids, "\rids=%d\r", ids);
		strcat(ptrTrama->parametros, pids);
		strcpy(tbsockets[idx].estado, CLIENTE_OCUPADO);
	} else {
		initParametros(ptrTrama,0);
		strcpy(ptrTrama->parametros, "nfn=NoComandosPtes\r");
	}
	if (!mandaTrama(socket_c, ptrTrama)) {
		liberaMemoria(iph);
		liberaMemoria(ido);	
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	liberaMemoria(iph);
	liberaMemoria(ido);	
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: buscaComandos
//
//	Descripción:
//		Busca en la base de datos,comandos pendientes de ejecutar por el cliente
//	Parámetros:
//		- ido: Identificador del ordenador
//		- cmd: Parámetros del comando (Salida)
//		- ids: Identificador de la sesion(Salida)
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN buscaComandos(char *ido, TRAMA *ptrTrama, int *ids)
{
	char msglog[LONSTD], sqlstr[LONSQL];
	Database db;
	Table tbl;
	int lonprm;

	char modulo[] = "buscaComandos()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexión con la BD
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	sprintf(sqlstr,"SELECT sesion,parametros,length( parametros) as lonprm"\
			" FROM acciones WHERE idordenador=%s AND estado='%d' ORDER BY idaccion", ido, ACCION_INICIADA);
	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (tbl.ISEOF()) {
		db.Close();
		return (FALSE); // No hay comandos pendientes
	} else { // Busca entre todas las acciones de diversos ambitos
		if (!tbl.Get("sesion", *ids)) { // Toma identificador de la sesion
			tbl.GetErrorErrStr(msglog); // Error al acceder al registro
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if (!tbl.Get("lonprm", lonprm)) { // Toma parámetros del comando
			tbl.GetErrorErrStr(msglog); // Error al acceder al registro
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if(!initParametros(ptrTrama,lonprm+LONGITUD_PARAMETROS)){
			db.Close();
			errorLog(modulo, 3, FALSE);
			return (FALSE);
		}
		if (!tbl.Get("parametros", ptrTrama->parametros)) { // Toma parámetros del comando
			tbl.GetErrorErrStr(msglog); // Error al acceder al registro
			errorInfo(modulo, msglog);
			return (FALSE);
		}
	}
	db.Close();
	return (TRUE); // Hay comandos pendientes, se toma el primero de la cola
}
// ________________________________________________________________________________________________________
// Función: DisponibilidadComandos
//
//	Descripción:
//		Esta función habilita a un cliente para recibir comandos desde la consola
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
//
BOOLEAN DisponibilidadComandos(SOCKET *socket_c, TRAMA *ptrTrama) 
{
	char *iph, *tpc;
	int idx,port_old=0,port_new;
	char modulo[] = "DisponibilidadComandos()";
	


	iph = copiaParametro("iph",ptrTrama); // Toma ip
	if (!clienteExistente(iph, &idx)) { // Busca índice del cliente
		liberaMemoria(iph);
		errorLog(modulo, 47, FALSE);
		return (FALSE);
	}
	tpc = copiaParametro("tpc",ptrTrama); // Tipo de cliente (Plataforma y S.O.)
	strcpy(tbsockets[idx].estado, tpc);

	port_new=tomaPuerto(*socket_c);

	if(tbsockets[idx].sock!=INVALID_SOCKET){
		port_old=tomaPuerto(tbsockets[idx].sock);
		if(port_old!=port_new){
			close(tbsockets[idx].sock); // Cierra el socket si ya existia uno
		}
	}

	tbsockets[idx].sock = *socket_c;
	swcSocket = TRUE; // El socket permanece abierto para recibir comandos desde el servidor
	liberaMemoria(iph);
	liberaMemoria(tpc);		
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: respuestaEstandar
//
//	Descripción:
//		Esta función actualiza la base de datos con el resultado de la ejecución de un comando con seguimiento
//	Parámetros:
//		- res: resultado de la ejecución del comando
//		- der: Descripción del error si hubiese habido
//		- iph: Dirección IP
//		- ids: identificador de la sesión
//		- ido: Identificador del ordenador que notifica
//		- db: Objeto base de datos (operativo)
//		- tbl: Objeto tabla
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN respuestaEstandar(TRAMA *ptrTrama, char *iph, char *ido, Database db,
		Table tbl) {
	char msglog[LONSTD], sqlstr[LONSQL];
	char *res, *ids, *der;
	char fechafin[LONPRM];
	struct tm* st;
	int idaccion;
	char modulo[] = "respuestaEstandar()";

	ids = copiaParametro("ids",ptrTrama); // Toma identificador de la sesión

	if (ids == NULL) // No existe seguimiento de la acción
		return (TRUE);
		
	if (atoi(ids) == 0){ // No existe seguimiento de la acción
		liberaMemoria(ids);
		return (TRUE);
	}

	sprintf(sqlstr,
			"SELECT * FROM acciones WHERE idordenador=%s"
			" AND sesion=%s ORDER BY idaccion", ido,ids);

	liberaMemoria(ids);

	if (!db.Execute(sqlstr, tbl)) { // Error al consultar
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (tbl.ISEOF()) { // No existe registro de acciones
		errorLog(modulo, 31, FALSE);
		return (TRUE);
	}
	if (!tbl.Get("idaccion", idaccion)) { // Toma identificador de la accion
		tbl.GetErrorErrStr(msglog); // Error al acceder al registro
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	st = tomaHora();
	sprintf(fechafin, "%d/%d/%d %d:%d:%d", st->tm_year + 1900, st->tm_mon + 1,
			st->tm_mday, st->tm_hour, st->tm_min, st->tm_sec);

	res = copiaParametro("res",ptrTrama); // Toma resultado
	der = copiaParametro("der",ptrTrama); // Toma descripción del error (si hubiera habido)
	
	sprintf(sqlstr,
			"UPDATE acciones"\
			"   SET resultado='%s',estado='%d',fechahorafin='%s',descrinotificacion='%s'"\
			" WHERE idordenador=%s AND idaccion=%d",
			res, ACCION_FINALIZADA, fechafin, der, ido, idaccion);
			
	if (!db.Execute(sqlstr, tbl)) { // Error al actualizar
		liberaMemoria(res);
		liberaMemoria(der);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	
	liberaMemoria(der);
	
	if (atoi(res) == ACCION_FALLIDA) {
		liberaMemoria(res);
		return (FALSE); // Error en la ejecución del comando
	}

	liberaMemoria(res);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: enviaComando
//
//	Descripción:
//		Envía un comando a los clientes
//	Parámetros:
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//		- estado: Estado en el se deja al cliente mientras se ejecuta el comando
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN enviaComando(TRAMA* ptrTrama, const char *estado)
 {
	char *iph, *Ipes, *ptrIpes[MAXIMOS_CLIENTES];
	int i, idx, lon;
	char modulo[] = "enviaComando()";

	iph = copiaParametro("iph",ptrTrama); // Toma dirección/es IP
	lon = strlen(iph); // Calcula longitud de la cadena de direccion/es IPE/S
	Ipes = (char*) reservaMemoria(lon + 1);
	if (Ipes == NULL) {
		errorLog(modulo, 3, FALSE);
		return (FALSE);
	}
	
	strcpy(Ipes, iph); // Copia cadena de IPES
	liberaMemoria(iph);

	lon = splitCadena(ptrIpes, Ipes, ';');
	FINCADaINTRO(ptrTrama);
	for (i = 0; i < lon; i++) {
		if (clienteDisponible(ptrIpes[i], &idx)) { // Si el cliente puede recibir comandos
			strcpy(tbsockets[idx].estado, estado); // Actualiza el estado del cliente
			if (!mandaTrama(&tbsockets[idx].sock, ptrTrama)) {
				errorLog(modulo, 26, FALSE);
				return (FALSE);
			}
			//close(tbsockets[idx].sock); // Cierra el socket del cliente hasta nueva disponibilidad
		}
	}
	liberaMemoria(Ipes);
	return (TRUE);
}
//______________________________________________________________________________________________________
// Función: respuestaConsola
//
//	Descripción:
// 		Envia una respuesta a la consola sobre el resultado de la ejecución de un comando
//	Parámetros:
//		- socket_c: (Salida) Socket utilizado para el envío
//		- res: Resultado del envío del comando
// 	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN respuestaConsola(SOCKET *socket_c, TRAMA *ptrTrama, int res) {
	char modulo[] = "respuestaConsola()";
	initParametros(ptrTrama,0);
	sprintf(ptrTrama->parametros, "res=%d\r", res);
	if (!mandaTrama(socket_c, ptrTrama)) {
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Arrancar
//
//	Descripción:
//		Procesa el comando Arrancar
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Arrancar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char *iph,*mac,*mar, msglog[LONSTD];
	BOOLEAN res;
	char modulo[] = "Arrancar()";

	iph = copiaParametro("iph",ptrTrama); // Toma dirección/es IP
	mac = copiaParametro("mac",ptrTrama); // Toma dirección/es MAC
	mar = copiaParametro("mar",ptrTrama); // Método de arranque (Broadcast o Unicast)

	res=Levanta(iph,mac,mar);

	liberaMemoria(iph);
	liberaMemoria(mac);
	liberaMemoria(mar);

	if(!res){
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Levanta
//
//	Descripción:
//		Enciende ordenadores a través de la red cuyas macs se pasan como parámetro
//	Parámetros:
//		- iph: Cadena de direcciones ip separadas por ";"
//		- mac: Cadena de direcciones mac separadas por ";"
//		- mar: Método de arranque (1=Broadcast, 2=Unicast)
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Levanta(char* iph,char *mac, char* mar)
{
	char *ptrIP[MAXIMOS_CLIENTES],*ptrMacs[MAXIMOS_CLIENTES];
	int i, lon, res;
	SOCKET s;
	BOOLEAN bOpt;
	sockaddr_in local;
	char modulo[] = "Levanta()";

	/* Creación de socket para envío de magig packet */
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == SOCKET_ERROR) { // Error al crear el socket del servicio
		errorLog(modulo, 13, TRUE);
		return (FALSE);
	}
	bOpt = TRUE; // Pone el socket en modo Broadcast
	res = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *) &bOpt, sizeof(bOpt));
	if (res == SOCKET_ERROR) {
		errorLog(modulo, 48, TRUE);
		return (FALSE);
	}
	local.sin_family = AF_INET;
	local.sin_port = htons((short) PUERTO_WAKEUP);
	local.sin_addr.s_addr = htonl(INADDR_ANY); // cualquier interface
	if (bind(s, (sockaddr *) &local, sizeof(local)) == SOCKET_ERROR) {
		errorLog(modulo, 14, TRUE);
		exit(EXIT_FAILURE);
	}
	/* fin creación de socket */
	lon = splitCadena(ptrIP, iph, ';');
	lon = splitCadena(ptrMacs, mac, ';');
	for (i = 0; i < lon; i++) {
		if (!WakeUp(&s,ptrIP[i],ptrMacs[i],mar)) {
			errorLog(modulo, 49, TRUE);
			close(s);
			return (FALSE);
		}
	}
	close(s);
	return (TRUE);
}
//_____________________________________________________________________________________________________________
// Función: WakeUp
//
//	 Descripción:
//		Enciende el ordenador cuya MAC se pasa como parámetro
//	Parámetros:
//		- s : Socket para enviar trama magic packet
//		- iph : Cadena con la dirección ip
//		- mac : Cadena con la dirección mac en formato XXXXXXXXXXXX
//		- mar: Método de arranque (1=Broadcast, 2=Unicast)
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
//_____________________________________________________________________________________________________________
//
BOOLEAN WakeUp(SOCKET *s, char* iph,char *mac,char* mar)
{
	int i, res;
	char HDaddress_bin[6];
	struct {
		BYTE secuencia_FF[6];
		char macbin[16][6];
	} Trama_WakeUp;
	sockaddr_in WakeUpCliente;
	char modulo[] = "WakeUp()";

	for (i = 0; i < 6; i++) // Primera secuencia de la trama Wake Up (0xFFFFFFFFFFFF)
		Trama_WakeUp.secuencia_FF[i] = 0xFF;

	PasaHexBin(mac, HDaddress_bin); // Pasa a binario la MAC

	for (i = 0; i < 16; i++) // Segunda secuencia de la trama Wake Up , repetir 16 veces su la MAC
		memcpy(&Trama_WakeUp.macbin[i][0], &HDaddress_bin, 6);

	/* Creación de socket del cliente que recibe la trama magic packet */
	WakeUpCliente.sin_family = AF_INET;
	WakeUpCliente.sin_port = htons((short) PUERTO_WAKEUP);
	if(atoi(mar)==2)
		WakeUpCliente.sin_addr.s_addr = inet_addr(iph); //  Para hacerlo con IP
	else
		WakeUpCliente.sin_addr.s_addr = htonl(INADDR_BROADCAST); //  Para hacerlo con broadcast

	res = sendto(*s, (char *) &Trama_WakeUp, sizeof(Trama_WakeUp), 0,
			(sockaddr *) &WakeUpCliente, sizeof(WakeUpCliente));
	if (res == SOCKET_ERROR) {
		errorLog(modulo, 26, FALSE);
		return (FALSE);
	}
	return (TRUE);
}
//_____________________________________________________________________________________________________________
// Función: PasaHexBin
//
//	Descripción:
//		Convierte a binario una dirección mac desde una cadena con formato XXXXXXXXXXXX
//
//	Parámetros de entrada:
//		- cadena : Cadena con el contenido de la mac
//		- numero : la dirección mac convertida a binario (6 bytes)
//_____________________________________________________________________________________________________________
void PasaHexBin(char *cadena, char *numero) {
	int i, j, p;
	char matrizHex[] = "0123456789ABCDEF";
	char Ucadena[12], aux;

	for (i = 0; i < 12; i++)
		Ucadena[i] = toupper(cadena[i]);
	p = 0;
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 16; j++) {
			if (Ucadena[i] == matrizHex[j]) {
				if (i % 2) {
					aux = numero[p];
					aux = (aux << 4);
					numero[p] = j;
					numero[p] = numero[p] | aux;
					p++;
				} else
					numero[p] = j;
				break;
			}
		}
	}
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_Arrancar
//
//	Descripción:
//		Respuesta del cliente al comando Arrancar
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_Arrancar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	Database db;
	Table tbl;
	int i;
	char *iph, *ido;
	char *tpc;
	char modulo[] = "RESPUESTA_Arrancar()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	tpc = copiaParametro("tpc",ptrTrama); // Tipo de cliente (Plataforma y S.O.)
	if (clienteExistente(iph, &i)) // Actualiza estado
		strcpy(tbsockets[i].estado, tpc);
		
	liberaMemoria(iph);
	liberaMemoria(ido);
	liberaMemoria(tpc);
	
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Comando
//
//	Descripción:
//		Procesa un comando personalizado
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Comando(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "Comando()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_Comando
//
//	Descripción:
//		Respuesta del cliente al un comando personalizado
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_Comando(SOCKET *socket_c, TRAMA* ptrTrama)
 {
	char msglog[LONSTD];
	Database db;
	Table tbl;
	char *iph, *ido;
	char modulo[] = "RESPUESTA_Comando()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}
	liberaMemoria(iph);
	liberaMemoria(ido);
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Apagar
//
//	Descripción:
//		Procesa el comando Apagar
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Apagar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "Apagar()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_Apagar
//
//	Descripción:
//		Respuesta del cliente al comando Apagar
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_Apagar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	Database db;
	Table tbl;
	int i;
	char *iph, *ido;
	char modulo[] = "RESPUESTA_Apagar()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	if (clienteExistente(iph, &i)) // Actualiza estado
		strcpy(tbsockets[i].estado, CLIENTE_APAGADO);
	
	liberaMemoria(iph);
	liberaMemoria(ido);
	
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Reiniciar
//
//	Descripción:
//		Procesa el comando Reiniciar
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Reiniciar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "Reiniciar()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_Reiniciar
//
//	Descripción:
//		Respuesta del cliente al comando Reiniciar
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_Reiniciar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	Database db;
	Table tbl;
	int i;
	char *iph, *ido;
	char modulo[] = "RESPUESTA_Reiniciar()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	if (clienteExistente(iph, &i)) // Actualiza estado
		strcpy(tbsockets[i].estado, CLIENTE_APAGADO);
	
	liberaMemoria(iph);
	liberaMemoria(ido);

	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: IniciarSesion
//
//	Descripción:
//		Procesa el comando Iniciar Sesión
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN IniciarSesion(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "IniciarSesion()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_IniciarSesion
//
//	Descripción:
//		Respuesta del cliente al comando Iniciar Sesión
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_IniciarSesion(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	Database db;
	Table tbl;
	int i;
	char *iph, *ido;
	char modulo[] = "RESPUESTA_IniciarSesion()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	if (clienteExistente(iph, &i)) // Actualiza estado
		strcpy(tbsockets[i].estado, CLIENTE_APAGADO);
		
	liberaMemoria(iph);
	liberaMemoria(ido);
		
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: CrearImagen
//
//	Descripción:
//		Crea una imagen de una partición de un disco y la guarda o bien en un repositorio
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN CrearImagen(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "CrearImagen()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_CrearImagen
//
//	Descripción:
//		Respuesta del cliente al comando CrearImagen
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_CrearImagen(SOCKET *socket_c, TRAMA* ptrTrama) 
{
	char msglog[LONSTD];
	Database db;
	Table tbl;
	char *iph, *dsk, *par, *cpt, *ipr, *ido;
	char *idi;
	BOOLEAN res;
	char modulo[] = "RESPUESTA_CrearImagen()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	// Acciones posteriores
	idi = copiaParametro("idi",ptrTrama);
	dsk = copiaParametro("dsk",ptrTrama);
	par = copiaParametro("par",ptrTrama);
	cpt = copiaParametro("cpt",ptrTrama);
	ipr = copiaParametro("ipr",ptrTrama);

	res=actualizaCreacionImagen(db, tbl, idi, dsk, par, cpt, ipr, ido);

	liberaMemoria(idi);
	liberaMemoria(par);
	liberaMemoria(cpt);
	liberaMemoria(ipr);
	
	if(!res){
		errorLog(modulo, 94, FALSE);
		db.Close(); // Cierra conexión
		return (FALSE);
	}

	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: actualizaCreacionImagen
//
//	Descripción:
//		Esta función actualiza la base de datos con el resultado de la creación de una imagen
//	Parámetros:
//		- db: Objeto base de datos (ya operativo)
//		- tbl: Objeto tabla
//		- idi: Identificador de la imagen
//		- dsk: Disco de donde se creó
//		- par: Partición de donde se creó
//		- cpt: Código de partición
//		- ipr: Ip del repositorio
//		- ido: Identificador del ordenador modelo
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN actualizaCreacionImagen(Database db, Table tbl, char* idi, char* dsk,
	char* par, char* cpt, char* ipr, char *ido) {
	char msglog[LONSTD], sqlstr[LONSQL];
	char modulo[] = "actualizaCreacionImagen()";
	int idr,ifs;

	/* Toma identificador del repositorio correspondiente al ordenador modelo */
	snprintf(sqlstr, LONSQL,
			"SELECT repositorios.idrepositorio"
			"  FROM repositorios"
			"  LEFT JOIN ordenadores USING (idrepositorio)"
			" WHERE repositorios.ip='%s' AND ordenadores.idordenador=%s", ipr, ido);

	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idrepositorio", idr)) { // Toma dato
		tbl.GetErrorErrStr(msglog); // Error al acceder al registro
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	/* Toma identificador del perfilsoftware */
	snprintf(sqlstr, LONSQL,
			"SELECT idperfilsoft"
			"  FROM ordenadores_particiones"
			" WHERE idordenador=%s AND numdisk=%s AND numpar=%s", ido, dsk, par);

	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idperfilsoft", ifs)) { // Toma dato
		tbl.GetErrorErrStr(msglog); // Error al acceder al registro
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	/* Actualizar los datos de la imagen */
	snprintf(sqlstr, LONSQL,
		"UPDATE imagenes"
		"   SET idordenador=%s, numdisk=%s, numpar=%s, codpar=%s,"
		"       idperfilsoft=%d, idrepositorio=%d,"
		"       fechacreacion=NOW(), revision=revision+1"
		" WHERE idimagen=%s", ido, dsk, par, cpt, ifs, idr, idi);

	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	/* Actualizar los datos en el cliente */
	snprintf(sqlstr, LONSQL,
		"UPDATE ordenadores_particiones"
		"   SET idimagen=%s, revision=(SELECT revision FROM imagenes WHERE idimagen=%s),"
		"       fechadespliegue=NOW()"
		" WHERE idordenador=%s AND numdisk=%s AND numpar=%s",
		idi, idi, ido, dsk, par);
	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: CrearImagenBasica
//
//	Descripción:
//		Crea una imagen basica usando sincronización
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN CrearImagenBasica(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "CrearImagenBasica()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_CrearImagenBasica
//
//	Descripción:
//		Respuesta del cliente al comando CrearImagenBasica
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_CrearImagenBasica(SOCKET *socket_c, TRAMA* ptrTrama) {
	return(RESPUESTA_CrearImagen(socket_c,ptrTrama)); // La misma respuesta que la creación de imagen monolítica
}
// ________________________________________________________________________________________________________
// Función: CrearSoftIncremental
//
//	Descripción:
//		Crea una imagen incremental entre una partición de un disco y una imagen ya creada guardandola en el
//		mismo repositorio y en la misma carpeta donde está la imagen básica
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN CrearSoftIncremental(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "CrearSoftIncremental()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_CrearSoftIncremental
//
//	Descripción:
//		Respuesta del cliente al comando crearImagenDiferencial
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_CrearSoftIncremental(SOCKET *socket_c, TRAMA* ptrTrama)
{
	Database db;
	Table tbl;
	char *iph,*par,*ido,*idf;
	int ifs;
	char msglog[LONSTD],sqlstr[LONSQL];
	char modulo[] = "RESPUESTA_CrearSoftIncremental()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);	
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	par = copiaParametro("par",ptrTrama);

	/* Toma identificador del perfilsoftware creado por el inventario de software */
	sprintf(sqlstr,"SELECT idperfilsoft FROM ordenadores_particiones WHERE idordenador=%s AND numpar=%s",ido,par);
	
	liberaMemoria(iph);
	liberaMemoria(ido);	
	liberaMemoria(par);	
		
	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idperfilsoft", ifs)) { // Toma dato
		tbl.GetErrorErrStr(msglog); // Error al acceder al registro
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	/* Actualizar los datos de la imagen */
	idf = copiaParametro("idf",ptrTrama);
	sprintf(sqlstr,"UPDATE imagenes SET idperfilsoft=%d WHERE idimagen=%s",ifs,idf);
	liberaMemoria(idf);	
	
	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: actualizaCreacionSoftIncremental
//
//	Descripción:
//		Esta función actualiza la base de datos con el resultado de la creación de software incremental
//	Parámetros:
//		- db: Objeto base de datos (ya operativo)
//		- tbl: Objeto tabla
//		- idi: Identificador de la imagen
//		- idf: Identificador del software incremental
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN actualizaCreacionSoftIncremental(Database db, Table tbl, char* idi,char* idf)
{
	char msglog[LONSTD], sqlstr[LONSQL];
	char modulo[] = "actualizaCreacionSoftIncremental()";


	/* Comprueba si existe ya relación entre la imagen y el software incremental */
	sprintf(sqlstr, "SELECT * FROM imagenes_softincremental"
					" WHERE idimagen=%s AND idsoftincremental=%s", idi,idf);

	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.ISEOF())
		return (TRUE); // Ya existe relación

	// Crea relación entre la imagen y el software incremental
	sprintf(sqlstr,"INSERT INTO imagenes_softincremental (idimagen,idsoftincremental) VALUES (%s,%s)",idi,idf);
	if (!db.Execute(sqlstr, tbl)) { // Error al ejecutar la sentencia
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RestaurarImagen
//
//	Descripción:
//		Restaura una imagen en una partición
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RestaurarImagen(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "RestaurarImagen()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RestaurarImagenBasica
//
//	Descripción:
//		Restaura una imagen básica en una partición
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RestaurarImagenBasica(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "RestaurarImagenBasica()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RestaurarSoftIncremental
//
//	Descripción:
//		Restaura una imagen básica junto con software incremental en una partición
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RestaurarSoftIncremental(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "RestaurarSoftIncremental()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_RestaurarImagen
//
//	Descripción:
//		Respuesta del cliente al comando RestaurarImagen
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
//
BOOLEAN RESPUESTA_RestaurarImagen(SOCKET *socket_c, TRAMA* ptrTrama) 
{
	char msglog[LONSTD];
	Database db;
	Table tbl;
	BOOLEAN res;
	char *iph, *ido, *idi, *dsk, *par, *ifs, *cfg;
	char modulo[] = "RESPUESTA_RestaurarImagen()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);	
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	// Acciones posteriores
	idi = copiaParametro("idi",ptrTrama); // Toma identificador de la imagen
	dsk = copiaParametro("dsk",ptrTrama); // Número de disco
	par = copiaParametro("par",ptrTrama); // Número de partición
	ifs = copiaParametro("ifs",ptrTrama); // Identificador del perfil software contenido
	cfg = copiaParametro("cfg",ptrTrama); // Configuración de discos
	if(cfg){
		actualizaConfiguracion(db, tbl, cfg, atoi(ido)); // Actualiza la configuración del ordenador
		liberaMemoria(cfg);	
	}
	res=actualizaRestauracionImagen(db, tbl, idi, dsk, par, ido, ifs);
	
	liberaMemoria(iph);
	liberaMemoria(ido);
	liberaMemoria(idi);
	liberaMemoria(par);
	liberaMemoria(ifs);

	if(!res){
		errorLog(modulo, 95, FALSE);
		db.Close(); // Cierra conexión
		return (FALSE);
	}

	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
//
// Función: RESPUESTA_RestaurarImagenBasica
//
//	Descripción:
//		Respuesta del cliente al comando RestaurarImagen
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
//
BOOLEAN RESPUESTA_RestaurarImagenBasica(SOCKET *socket_c, TRAMA* ptrTrama) {
	return(RESPUESTA_RestaurarImagen(socket_c,ptrTrama));
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_RestaurarSoftIncremental
//
//	Descripción:
//		Respuesta del cliente al comando RestaurarSoftIncremental
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_RestaurarSoftIncremental(SOCKET *socket_c, TRAMA* ptrTrama) {
	return(RESPUESTA_RestaurarImagen(socket_c,ptrTrama));
}
// ________________________________________________________________________________________________________
// Función: actualizaRestauracionImagen
//
//	Descripción:
//		Esta función actualiza la base de datos con el resultado de la restauración de una imagen
//	Parámetros:
//		- db: Objeto base de datos (ya operativo)
//		- tbl: Objeto tabla
//		- idi: Identificador de la imagen
//		- dsk: Disco de donde se restauró
//		- par: Partición de donde se restauró
//		- ido: Identificador del cliente donde se restauró
//		- ifs: Identificador del perfil software contenido	en la imagen
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN actualizaRestauracionImagen(Database db, Table tbl, char* idi,
	char* dsk, char* par, char* ido, char* ifs) {
	char msglog[LONSTD], sqlstr[LONSQL];
	char modulo[] = "actualizaRestauracionImagen()";

	/* Actualizar los datos de la imagen */
	snprintf(sqlstr, LONSQL,
			"UPDATE ordenadores_particiones"
			"   SET idimagen=%s, idperfilsoft=%s, fechadespliegue=NOW(),"
			"       revision=(SELECT revision FROM imagenes WHERE idimagen=%s)," 
			"       idnombreso=(SELECT idnombreso FROM perfilessoft WHERE idperfilsoft=%s)" 
			" WHERE idordenador=%s AND numdisk=%s AND numpar=%s", idi, ifs, idi, ifs, ido, dsk, par);

	if (!db.Execute(sqlstr, tbl)) { // Error al recuperar los datos
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: Configurar
//
//	Descripción:
//		Configura la tabla de particiones
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN Configurar(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "Configurar()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_Configurar
//
//	Descripción:
//		Respuesta del cliente al comando Configurar
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
//
BOOLEAN RESPUESTA_Configurar(SOCKET *socket_c, TRAMA* ptrTrama) 
{
	char msglog[LONSTD];
	Database db;
	Table tbl;
	BOOLEAN res;
	char *iph, *ido,*cfg;
	char modulo[] = "RESPUESTA_Configurar()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);	
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}

	cfg = copiaParametro("cfg",ptrTrama); // Toma configuración de particiones
	res=actualizaConfiguracion(db, tbl, cfg, atoi(ido)); // Actualiza la configuración del ordenador
	
	liberaMemoria(iph);
	liberaMemoria(ido);	
	liberaMemoria(cfg);	
		
	if(!res){	
		errorLog(modulo, 24, FALSE);
		return (FALSE); // Error al registrar notificacion
	}
	
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: EjecutarScript
//
//	Descripción:
//		Ejecuta un script de código
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN EjecutarScript(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "EjecutarScript()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_EjecutarScript
//
//	Descripción:
//		Respuesta del cliente al comando EjecutarScript
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_EjecutarScript(SOCKET *socket_c, TRAMA* ptrTrama)
{
	char msglog[LONSTD];
	Database db;
	Table tbl;
	char *iph, *ido,*cfg;

	char modulo[] = "RESPUESTA_EjecutarScript()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);	
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}
	
	cfg = copiaParametro("cfg",ptrTrama); // Toma configuración de particiones
	if(cfg){
		actualizaConfiguracion(db, tbl, cfg, atoi(ido)); // Actualiza la configuración del ordenador
		liberaMemoria(cfg);	
	}

	liberaMemoria(iph);
	liberaMemoria(ido);

	
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: InventarioHardware
//
//	Descripción:
//		Solicita al cliente un inventario de su hardware
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN InventarioHardware(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "InventarioHardware()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_InventarioHardware
//
//	Descripción:
//		Respuesta del cliente al comando InventarioHardware
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_InventarioHardware(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	Database db;
	Table tbl;
	BOOLEAN res;
	char *iph, *ido, *idc, *npc, *hrd, *buffer;
	char modulo[] = "RESPUESTA_InventarioHardware()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip del cliente
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del cliente

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);	
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}
	// Lee archivo de inventario enviado anteriormente
	hrd = copiaParametro("hrd",ptrTrama);
	buffer = rTrim(leeArchivo(hrd));
	
	npc = copiaParametro("npc",ptrTrama); 
	idc = copiaParametro("idc",ptrTrama); // Toma identificador del Centro
	
	if (buffer) 
		res=actualizaHardware(db, tbl, buffer, ido, npc, idc);
	
	liberaMemoria(iph);
	liberaMemoria(ido);			
	liberaMemoria(npc);			
	liberaMemoria(idc);		
	liberaMemoria(buffer);		
	
	if(!res){
		errorLog(modulo, 53, FALSE);
		return (FALSE);
	}
		
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: actualizaHardware
//
//		Descripción:
//			Actualiza la base de datos con la configuracion hardware del cliente
//		Parámetros:
//			- db: Objeto base de datos (ya operativo)
//			- tbl: Objeto tabla
//			- hrd: cadena con el inventario hardware
//			- ido: Identificador del ordenador
//			- npc: Nombre del ordenador
//			- idc: Identificador del centro o Unidad organizativa
// ________________________________________________________________________________________________________
//
BOOLEAN actualizaHardware(Database db, Table tbl, char* hrd, char*ido,char* npc, char *idc) 
{
	char msglog[LONSTD], sqlstr[LONSQL];
	int idtipohardware, idperfilhard;
	int lon, i, j, aux;
	bool retval;
	char *whard;
	int tbidhardware[MAXHARDWARE];
	char *tbHardware[MAXHARDWARE],*dualHardware[2], descripcion[250], strInt[LONINT], *idhardwares;
	char modulo[] = "actualizaHardware()";

	/* Toma Centro (Unidad Organizativa) */
	sprintf(sqlstr, "SELECT * FROM ordenadores WHERE idordenador=%s", ido);

	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	if (!tbl.Get("idperfilhard", idperfilhard)) { // Toma dato
		tbl.GetErrorErrStr(msglog); // Error al acceder al registro
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	whard=escaparCadena(hrd); // Codificar comillas simples
	if(!whard)
		return (FALSE);
	/* Recorre componentes hardware*/
	lon = splitCadena(tbHardware, whard, '\n');
	if (lon > MAXHARDWARE)
		lon = MAXHARDWARE; // Limita el número de componentes hardware
	/*
	 for (i=0;i<lon;i++){
	 sprintf(msglog,"Linea de inventario: %s",tbHardware[i]);
	 RegistraLog(msglog,FALSE);
	 }
	 */
	for (i = 0; i < lon; i++) {
		splitCadena(dualHardware, rTrim(tbHardware[i]), '=');
		//sprintf(msglog,"nemonico: %s",dualHardware[0]);
		//RegistraLog(msglog,FALSE);
		//sprintf(msglog,"valor: %s",dualHardware[1]);
		//RegistraLog(msglog,FALSE);
		sprintf(sqlstr, "SELECT idtipohardware,descripcion FROM tipohardwares "
			" WHERE nemonico='%s'", dualHardware[0]);
		if (!db.Execute(sqlstr, tbl)) { // Error al leer
			errorLog(modulo, 21, FALSE);
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if (tbl.ISEOF()) { //  Tipo de Hardware NO existente
			sprintf(msglog, "%s: %s)", tbErrores[54], dualHardware[0]);
			errorInfo(modulo, msglog);
			return (FALSE);
		} else { //  Tipo de Hardware Existe
			if (!tbl.Get("idtipohardware", idtipohardware)) { // Toma dato
				tbl.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (FALSE);
			}
			if (!tbl.Get("descripcion", descripcion)) { // Toma dato
				tbl.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (FALSE);
			}

			sprintf(sqlstr, "SELECT idhardware FROM hardwares "
				" WHERE idtipohardware=%d AND descripcion='%s'",
					idtipohardware, dualHardware[1]);

			// Ejecuta consulta
			if (!db.Execute(sqlstr, tbl)) { // Error al leer
				errorLog(modulo, 21, FALSE);
				db.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				return (FALSE);
			}

			if (tbl.ISEOF()) { //  Hardware NO existente
				sprintf(sqlstr, "INSERT hardwares (idtipohardware,descripcion,idcentro,grupoid) "
							" VALUES(%d,'%s',%s,0)", idtipohardware,
						dualHardware[1], idc);
				if (!db.Execute(sqlstr, tbl)) { // Error al insertar
					db.GetErrorErrStr(msglog); // Error al acceder al registro
					errorInfo(modulo, msglog);
					return (FALSE);
				}
				// Recupera el identificador del hardware
				sprintf(sqlstr, "SELECT LAST_INSERT_ID() as identificador");
				if (!db.Execute(sqlstr, tbl)) { // Error al leer
					errorLog(modulo, 21, FALSE);
					db.GetErrorErrStr(msglog);
					errorInfo(modulo, msglog);
					return (FALSE);
				}
				if (!tbl.ISEOF()) { // Si existe registro
					if (!tbl.Get("identificador", tbidhardware[i])) {
						tbl.GetErrorErrStr(msglog); // Error al acceder al registro
						errorInfo(modulo, msglog);
						return (FALSE);
					}
				}
			} else {
				if (!tbl.Get("idhardware", tbidhardware[i])) { // Toma dato
					tbl.GetErrorErrStr(msglog); // Error al acceder al registro
					errorInfo(modulo, msglog);
					return (FALSE);
				}
			}
		}
	}
	// Ordena tabla de identificadores para cosultar si existe un pefil con esas especificaciones

	for (i = 0; i < lon - 1; i++) {
		for (j = i + 1; j < lon; j++) {
			if (tbidhardware[i] > tbidhardware[j]) {
				aux = tbidhardware[i];
				tbidhardware[i] = tbidhardware[j];
				tbidhardware[j] = aux;
			}
		}
	}
	/* Crea cadena de identificadores de componentes hardware separados por coma */
	sprintf(strInt, "%d", tbidhardware[lon - 1]); // Pasa a cadena el último identificador que es de mayor longitud
	aux = strlen(strInt); // Calcula longitud de cadena para reservar espacio a todos los perfiles
	idhardwares = reservaMemoria(sizeof(aux) * lon + lon);
	if (idhardwares == NULL) {
		errorLog(modulo, 3, FALSE);
		return (FALSE);
	}
	aux = sprintf(idhardwares, "%d", tbidhardware[0]);
	for (i = 1; i < lon; i++)
		aux += sprintf(idhardwares + aux, ",%d", tbidhardware[i]);

	if (!cuestionPerfilHardware(db, tbl, idc, ido, idperfilhard, idhardwares,
			npc, tbidhardware, lon)) {
		errorLog(modulo, 55, FALSE);
		errorInfo(modulo, msglog);
		retval=FALSE;
	}
	else {
		retval=TRUE;
	}
	liberaMemoria(whard);
	liberaMemoria(idhardwares);
	return (retval);
}
// ________________________________________________________________________________________________________
// Función: cuestionPerfilHardware
//
//		Descripción:
//			Comprueba existencia de perfil hardware y actualización de éste para el ordenador
//		Parámetros:
//			- db: Objeto base de datos (ya operativo)
//			- tbl: Objeto tabla
//			- idc: Identificador de la Unidad organizativa donde se encuentra el cliente
//			- ido: Identificador del ordenador
//			- tbidhardware: Identificador del tipo de hardware
//			- con: Número de componentes detectados para configurar un el perfil hardware
//			- npc: Nombre del cliente
// ________________________________________________________________________________________________________
BOOLEAN cuestionPerfilHardware(Database db, Table tbl, char* idc, char* ido,
		int idperfilhardware, char*idhardwares, char *npc, int *tbidhardware,
		int lon)
{
	char msglog[LONSTD], *sqlstr;
	int i;
	int nwidperfilhard;
	char modulo[] = "cuestionPerfilHardware()";

	sqlstr = reservaMemoria(strlen(idhardwares)+LONSQL); // Reserva para escribir sentencia SQL
	if (sqlstr == NULL) {
		errorLog(modulo, 3, FALSE);
		return (FALSE);
	}
	// Busca perfil hard del ordenador que contenga todos los componentes hardware encontrados
	sprintf(sqlstr, "SELECT idperfilhard FROM"
		" (SELECT perfileshard_hardwares.idperfilhard as idperfilhard,"
		"	group_concat(cast(perfileshard_hardwares.idhardware AS char( 11) )"
		"	ORDER BY perfileshard_hardwares.idhardware SEPARATOR ',' ) AS idhardwares"
		" FROM	perfileshard_hardwares"
		" GROUP BY perfileshard_hardwares.idperfilhard) AS temp"
		" WHERE idhardwares LIKE '%s'", idhardwares);
	// Ejecuta consulta
	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}
	if (tbl.ISEOF()) { // No existe un perfil hardware con esos componentes de componentes hardware, lo crea
		sprintf(sqlstr, "INSERT perfileshard  (descripcion,idcentro,grupoid)"
				" VALUES('Perfil hardware (%s) ',%s,0)", npc, idc);
		if (!db.Execute(sqlstr, tbl)) { // Error al insertar
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			liberaMemoria(sqlstr);
			return (false);
		}
		// Recupera el identificador del nuevo perfil hardware
		sprintf(sqlstr, "SELECT LAST_INSERT_ID() as identificador");
		if (!db.Execute(sqlstr, tbl)) { // Error al leer
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			liberaMemoria(sqlstr);
			return (false);
		}
		if (!tbl.ISEOF()) { // Si existe registro
			if (!tbl.Get("identificador", nwidperfilhard)) {
				tbl.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				liberaMemoria(sqlstr);
				return (false);
			}
		}
		// Crea la relación entre perfiles y componenetes hardware
		for (i = 0; i < lon; i++) {
			sprintf(sqlstr, "INSERT perfileshard_hardwares  (idperfilhard,idhardware)"
						" VALUES(%d,%d)", nwidperfilhard, tbidhardware[i]);
			if (!db.Execute(sqlstr, tbl)) { // Error al insertar
				db.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				liberaMemoria(sqlstr);
				return (false);
			}
		}
	} else { // Existe un perfil con todos esos componentes
		if (!tbl.Get("idperfilhard", nwidperfilhard)) {
			tbl.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			liberaMemoria(sqlstr);
			return (false);
		}
	}
	if (idperfilhardware != nwidperfilhard) { // No coinciden los perfiles
		// Actualiza el identificador del perfil hardware del ordenador
		sprintf(sqlstr, "UPDATE ordenadores SET idperfilhard=%d"
			" WHERE idordenador=%s", nwidperfilhard, ido);
		if (!db.Execute(sqlstr, tbl)) { // Error al insertar
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			liberaMemoria(sqlstr);
			return (false);
		}
	}
	/* Eliminar Relación de hardwares con Perfiles hardware que quedan húerfanos */
	sprintf(sqlstr, "DELETE FROM perfileshard_hardwares WHERE idperfilhard IN "
		" (SELECT idperfilhard FROM perfileshard WHERE idperfilhard NOT IN"
		" (SELECT DISTINCT idperfilhard from ordenadores))");
	if (!db.Execute(sqlstr, tbl)) { // Error al insertar
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}

	/* Eliminar Perfiles hardware que quedan húerfanos */
	sprintf(sqlstr, "DELETE FROM perfileshard WHERE idperfilhard NOT IN"
			" (SELECT DISTINCT idperfilhard FROM ordenadores)");
	if (!db.Execute(sqlstr, tbl)) { // Error al insertar
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}
	/* Eliminar Relación de hardwares con Perfiles hardware que quedan húerfanos */
	sprintf(sqlstr, "DELETE FROM perfileshard_hardwares WHERE idperfilhard NOT IN"
			" (SELECT idperfilhard FROM perfileshard)");
	if (!db.Execute(sqlstr, tbl)) { // Error al insertar
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}
	liberaMemoria(sqlstr);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: InventarioSoftware
//
//	Descripción:
//		Solicita al cliente un inventario de su software
//	Parámetros:
//		- socket_c: Socket de la consola al envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN InventarioSoftware(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	char modulo[] = "InventarioSoftware()";

	if (!enviaComando(ptrTrama, CLIENTE_OCUPADO)) {
		sprintf(msglog, "%s:%s", tbErrores[32], modulo);
		errorInfo(modulo, msglog);
		respuestaConsola(socket_c, ptrTrama, FALSE);
		return (FALSE);
	}
	respuestaConsola(socket_c, ptrTrama, TRUE);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: RESPUESTA_InventarioSoftware
//
//	Descripción:
//		Respuesta del cliente al comando InventarioSoftware
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN RESPUESTA_InventarioSoftware(SOCKET *socket_c, TRAMA* ptrTrama) {
	char msglog[LONSTD];
	Database db;
	Table tbl;
	BOOLEAN res;
	char *iph, *ido, *npc, *idc, *par, *sft, *buffer;
	char modulo[] = "RESPUESTA_InventarioSoftware()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	iph = copiaParametro("iph",ptrTrama); // Toma dirección ip
	ido = copiaParametro("ido",ptrTrama); // Toma identificador del ordenador

	if (!respuestaEstandar(ptrTrama, iph, ido, db, tbl)) {
		liberaMemoria(iph);
		liberaMemoria(ido);	
		errorLog(modulo, 30, FALSE);
		return (FALSE); // Error al registrar notificacion
	}
	
	npc = copiaParametro("npc",ptrTrama); 
	idc = copiaParametro("idc",ptrTrama); // Toma identificador del Centro	
	par = copiaParametro("par",ptrTrama);
	sft = copiaParametro("sft",ptrTrama);

	buffer = rTrim(leeArchivo(sft));
	if (buffer)
		res=actualizaSoftware(db, tbl, buffer, par, ido, npc, idc);

	liberaMemoria(iph);
	liberaMemoria(ido);	
	liberaMemoria(npc);	
	liberaMemoria(idc);	
	liberaMemoria(par);	
	liberaMemoria(sft);	

	if(!res){
		errorLog(modulo, 82, FALSE);
		return (FALSE);
	}	
	
	db.Close(); // Cierra conexión
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: actualizaSoftware
//
//	Descripción:
//		Actualiza la base de datos con la configuración software del cliente
//	Parámetros:
//		- db: Objeto base de datos (ya operativo)
//		- tbl: Objeto tabla
//		- sft: cadena con el inventario software
//		- par: Número de la partición
//		- ido: Identificador del ordenador del cliente en la tabla
//		- npc: Nombre del ordenador
//		- idc: Identificador del centro o Unidad organizativa
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
//
//	Versión 1.1.0: Se incluye el sistema operativo. Autora: Irina Gómez - ETSII Universidad Sevilla
// ________________________________________________________________________________________________________
BOOLEAN actualizaSoftware(Database db, Table tbl, char* sft, char* par,char* ido, char* npc, char* idc) 
{
	int i, j, lon, aux, idperfilsoft, idnombreso;
	bool retval;
	char *wsft;
	int tbidsoftware[MAXSOFTWARE];
	char *tbSoftware[MAXSOFTWARE],msglog[LONSTD], sqlstr[LONSQL], strInt[LONINT], *idsoftwares;
	char modulo[] = "actualizaSoftware()";

	/* Toma Centro (Unidad Organizativa) y perfil software */
	sprintf(sqlstr, "SELECT idperfilsoft,numpar"
		" FROM ordenadores_particiones"
		" WHERE idordenador=%s", ido);

	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	idperfilsoft = 0; // Por defecto se supone que el ordenador no tiene aún detectado el perfil software
	while (!tbl.ISEOF()) { // Recorre particiones
		if (!tbl.Get("numpar", aux)) {
			tbl.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if (aux == atoi(par)) { // Se encuentra la partición
			if (!tbl.Get("idperfilsoft", idperfilsoft)) {
				tbl.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				return (FALSE);
			}
			break;
		}
		tbl.MoveNext();
	}
	wsft=escaparCadena(sft); // Codificar comillas simples
	if(!wsft)
		return (FALSE);

	/* Recorre componentes software*/
	lon = splitCadena(tbSoftware, wsft, '\n');

	if (lon == 0)
		return (true); // No hay lineas que procesar
	if (lon > MAXSOFTWARE)
		lon = MAXSOFTWARE; // Limita el número de componentes software

	for (i = 0; i < lon; i++) {
		// Primera línea es el sistema operativo: se obtiene identificador
		if (i == 0) {
			idnombreso = checkDato(db, tbl, rTrim(tbSoftware[i]), "nombresos", "nombreso", "idnombreso");
			continue;
		}

		sprintf(sqlstr,
				"SELECT idsoftware FROM softwares WHERE descripcion ='%s'",
				rTrim(tbSoftware[i]));

		// Ejecuta consulta
		if (!db.Execute(sqlstr, tbl)) { // Error al leer
			errorLog(modulo, 21, FALSE);
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			return (FALSE);
		}

		if (tbl.ISEOF()) { //  Software NO existente
			sprintf(sqlstr, "INSERT INTO softwares (idtiposoftware,descripcion,idcentro,grupoid)"
						" VALUES(2,'%s',%s,0)", tbSoftware[i], idc);

			if (!db.Execute(sqlstr, tbl)) { // Error al insertar
				db.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (FALSE);
			}
			// Recupera el identificador del software
			sprintf(sqlstr, "SELECT LAST_INSERT_ID() as identificador");
			if (!db.Execute(sqlstr, tbl)) { // Error al leer
				db.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (FALSE);
			}
			if (!tbl.ISEOF()) { // Si existe registro
				if (!tbl.Get("identificador", tbidsoftware[i])) {
					tbl.GetErrorErrStr(msglog); // Error al acceder al registro
					errorInfo(modulo, msglog);
					return (FALSE);
				}
			}
		} else {
			if (!tbl.Get("idsoftware", tbidsoftware[i])) { // Toma dato
				tbl.GetErrorErrStr(msglog); // Error al acceder al registro
				errorInfo(modulo, msglog);
				return (FALSE);
			}
		}
	}

	// Ordena tabla de identificadores para cosultar si existe un pefil con esas especificaciones

	for (i = 0; i < lon - 1; i++) {
		for (j = i + 1; j < lon; j++) {
			if (tbidsoftware[i] > tbidsoftware[j]) {
				aux = tbidsoftware[i];
				tbidsoftware[i] = tbidsoftware[j];
				tbidsoftware[j] = aux;
			}
		}
	}
	/* Crea cadena de identificadores de componentes software separados por coma */
	sprintf(strInt, "%d", tbidsoftware[lon - 1]); // Pasa a cadena el último identificador que es de mayor longitud
	aux = strlen(strInt); // Calcula longitud de cadena para reservar espacio a todos los perfiles
	idsoftwares = reservaMemoria((sizeof(aux)+1) * lon + lon);
	if (idsoftwares == NULL) {
		errorLog(modulo, 3, FALSE);
		return (FALSE);
	}
	aux = sprintf(idsoftwares, "%d", tbidsoftware[0]);
	for (i = 1; i < lon; i++)
		aux += sprintf(idsoftwares + aux, ",%d", tbidsoftware[i]);

	// Comprueba existencia de perfil software y actualización de éste para el ordenador
	if (!cuestionPerfilSoftware(db, tbl, idc, ido, idperfilsoft, idnombreso, idsoftwares, 
			npc, par, tbidsoftware, lon)) {
		errorLog(modulo, 83, FALSE);
		errorInfo(modulo, msglog);
		retval=FALSE;
	}
	else {
		retval=TRUE;
	}
	liberaMemoria(wsft);
	liberaMemoria(idsoftwares);
	return (retval);
}
// ________________________________________________________________________________________________________
// Función: CuestionPerfilSoftware
//
//	Parámetros:
//		- db: Objeto base de datos (ya operativo)
//		- tbl: Objeto tabla
//		- idcentro: Identificador del centro en la tabla
//		- ido: Identificador del ordenador del cliente en la tabla
//		- idnombreso: Identificador del sistema operativo
//		- idsoftwares: Cadena con los identificadores de componentes software separados por comas
//		- npc: Nombre del ordenador del cliente
//		- particion: Número de la partición
//		- tbidsoftware: Array con los identificadores de componentes software
//		- lon: Número de componentes
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
//
//	Versión 1.1.0: Se incluye el sistema operativo. Autora: Irina Gómez - ETSII Universidad Sevilla
//_________________________________________________________________________________________________________
BOOLEAN cuestionPerfilSoftware(Database db, Table tbl, char* idc, char* ido,
		int idperfilsoftware, int idnombreso, char *idsoftwares, char *npc, char *par,
		int *tbidsoftware, int lon) {
	char *sqlstr, msglog[LONSTD];
	int i, nwidperfilsoft;
	char modulo[] = "cuestionPerfilSoftware()";

	sqlstr = reservaMemoria(strlen(idsoftwares)+LONSQL); // Reserva para escribir sentencia SQL
	if (sqlstr == NULL) {
		errorLog(modulo, 3, FALSE);
		return (FALSE);
	}
	// Busca perfil soft del ordenador que contenga todos los componentes software encontrados
	sprintf(sqlstr, "SELECT idperfilsoft FROM"
		" (SELECT perfilessoft_softwares.idperfilsoft as idperfilsoft,"
		"	group_concat(cast(perfilessoft_softwares.idsoftware AS char( 11) )"
		"	ORDER BY perfilessoft_softwares.idsoftware SEPARATOR ',' ) AS idsoftwares"
		" FROM	perfilessoft_softwares"
		" GROUP BY perfilessoft_softwares.idperfilsoft) AS temp"
		" WHERE idsoftwares LIKE '%s'", idsoftwares);
	// Ejecuta consulta
	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}
	if (tbl.ISEOF()) { // No existe un perfil software con esos componentes de componentes software, lo crea
		sprintf(sqlstr, "INSERT perfilessoft  (descripcion, idcentro, grupoid, idnombreso)"
				" VALUES('Perfil Software (%s, Part:%s) ',%s,0,%i)", npc, par, idc,idnombreso);
		if (!db.Execute(sqlstr, tbl)) { // Error al insertar
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			return (false);
		}
		// Recupera el identificador del nuevo perfil software
		sprintf(sqlstr, "SELECT LAST_INSERT_ID() as identificador");
		if (!db.Execute(sqlstr, tbl)) { // Error al leer
			tbl.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			liberaMemoria(sqlstr);
			return (false);
		}
		if (!tbl.ISEOF()) { // Si existe registro
			if (!tbl.Get("identificador", nwidperfilsoft)) {
				tbl.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				liberaMemoria(sqlstr);
				return (false);
			}
		}
		// Crea la relación entre perfiles y componenetes software
		for (i = 0; i < lon; i++) {
			sprintf(sqlstr, "INSERT perfilessoft_softwares (idperfilsoft,idsoftware)"
						" VALUES(%d,%d)", nwidperfilsoft, tbidsoftware[i]);
			if (!db.Execute(sqlstr, tbl)) { // Error al insertar
				db.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				liberaMemoria(sqlstr);
				return (false);
			}
		}
	} else { // Existe un perfil con todos esos componentes
		if (!tbl.Get("idperfilsoft", nwidperfilsoft)) {
			tbl.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			liberaMemoria(sqlstr);
			return (false);
		}
	}

	if (idperfilsoftware != nwidperfilsoft) { // No coinciden los perfiles
		// Actualiza el identificador del perfil software del ordenador
		sprintf(sqlstr, "UPDATE ordenadores_particiones SET idperfilsoft=%d,idimagen=0"
				" WHERE idordenador=%s AND numpar=%s", nwidperfilsoft, ido, par);
		if (!db.Execute(sqlstr, tbl)) { // Error al insertar
			db.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			liberaMemoria(sqlstr);
			return (false);
		}
	}

	/* DEPURACIÓN DE PERFILES SOFTWARE */

	 /* Eliminar Relación de softwares con Perfiles software que quedan húerfanos */
	sprintf(sqlstr, "DELETE FROM perfilessoft_softwares WHERE idperfilsoft IN "\
		" (SELECT idperfilsoft FROM perfilessoft WHERE idperfilsoft NOT IN"\
		" (SELECT DISTINCT idperfilsoft from ordenadores_particiones) AND idperfilsoft NOT IN"\
		" (SELECT DISTINCT idperfilsoft from imagenes))");
	if (!db.Execute(sqlstr, tbl)) { // Error al insertar
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}
	/* Eliminar Perfiles software que quedan húerfanos */
	sprintf(sqlstr, "DELETE FROM perfilessoft WHERE idperfilsoft NOT IN"
		" (SELECT DISTINCT idperfilsoft from ordenadores_particiones)"\
		" AND  idperfilsoft NOT IN"\
		" (SELECT DISTINCT idperfilsoft from imagenes)");
	if (!db.Execute(sqlstr, tbl)) { // Error al insertar
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}
	/* Eliminar Relación de softwares con Perfiles software que quedan húerfanos */
	sprintf(sqlstr, "DELETE FROM perfilessoft_softwares WHERE idperfilsoft NOT IN"
			" (SELECT idperfilsoft from perfilessoft)");
	if (!db.Execute(sqlstr, tbl)) { // Error al insertar
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		liberaMemoria(sqlstr);
		return (false);
	}
	liberaMemoria(sqlstr);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: enviaArchivo
//
//	Descripción:
//		Envia un archivo por la red, por bloques
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN enviaArchivo(SOCKET *socket_c, TRAMA *ptrTrama) {
	char *nfl;
	char modulo[] = "enviaArchivo()";

	// Toma parámetros
	nfl = copiaParametro("nfl",ptrTrama); // Toma nombre completo del archivo
	if (!sendArchivo(socket_c, nfl)) {
		liberaMemoria(nfl);
		errorLog(modulo, 57, FALSE);
		return (FALSE);
	}
	liberaMemoria(nfl);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: enviaArchivo
//
//	Descripción:
//		Envia un archivo por la red, por bloques
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN recibeArchivo(SOCKET *socket_c, TRAMA *ptrTrama) {
	char *nfl;
	char modulo[] = "recibeArchivo()";

	// Toma parámetros
	nfl = copiaParametro("nfl",ptrTrama); // Toma nombre completo del archivo
	ptrTrama->tipo = MSG_NOTIFICACION;
	enviaFlag(socket_c, ptrTrama);
	if (!recArchivo(socket_c, nfl)) {
		liberaMemoria(nfl);
		errorLog(modulo, 58, FALSE);
		return (FALSE);
	}
	liberaMemoria(nfl);
	return (TRUE);
}
// ________________________________________________________________________________________________________
// Función: envioProgramacion
//
//	Descripción:
//		Envia un comando de actualización a todos los ordenadores que han sido programados con
//		alguna acción para que entren en el bucle de comandos pendientes y las ejecuten
//	Parámetros:
//		- socket_c: Socket del cliente que envió el mensaje
//		- ptrTrama: Trama recibida por el servidor con el contenido y los parámetros
//	Devuelve:
//		TRUE: Si el proceso es correcto
//		FALSE: En caso de ocurrir algún error
// ________________________________________________________________________________________________________
BOOLEAN envioProgramacion(SOCKET *socket_c, TRAMA *ptrTrama)
{
	char sqlstr[LONSQL], msglog[LONSTD];
	char *idp,iph[LONIP],mac[LONMAC];
	Database db;
	Table tbl;
	int idx,idcomando;
	char modulo[] = "envioProgramacion()";

	if (!db.Open(usuario, pasguor, datasource, catalog)) { // Error de conexion
		errorLog(modulo, 20, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}

	idp = copiaParametro("idp",ptrTrama); // Toma identificador de la programación de la tabla acciones

	sprintf(sqlstr, "SELECT ordenadores.ip,ordenadores.mac,acciones.idcomando FROM acciones "\
			" INNER JOIN ordenadores ON ordenadores.ip=acciones.ip"\
			" WHERE acciones.idprogramacion=%s",idp);
	
	liberaMemoria(idp);
			
	if (!db.Execute(sqlstr, tbl)) { // Error al leer
		errorLog(modulo, 21, FALSE);
		db.GetErrorErrStr(msglog);
		errorInfo(modulo, msglog);
		return (FALSE);
	}
	db.Close();
	if(tbl.ISEOF())
		return (TRUE); // No existen registros

	/* Prepara la trama de actualizacion */

	initParametros(ptrTrama,0);
	ptrTrama->tipo=MSG_COMANDO;
	sprintf(ptrTrama->parametros, "nfn=Actualizar\r");

	while (!tbl.ISEOF()) { // Recorre particiones
		if (!tbl.Get("ip", iph)) {
			tbl.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if (!tbl.Get("idcomando", idcomando)) {
			tbl.GetErrorErrStr(msglog);
			errorInfo(modulo, msglog);
			return (FALSE);
		}
		if(idcomando==1){ // Arrancar
			if (!tbl.Get("mac", mac)) {
				tbl.GetErrorErrStr(msglog);
				errorInfo(modulo, msglog);
				return (FALSE);
			}

			// Se manda por broadcast y por unicast
			if (!Levanta(iph, mac, (char*)"1")) {
				sprintf(msglog, "%s:%s", tbErrores[32], modulo);
				errorInfo(modulo, msglog);
				return (FALSE);
			}

			if (!Levanta(iph, mac, (char*)"2")) {
				sprintf(msglog, "%s:%s", tbErrores[32], modulo);
				errorInfo(modulo, msglog);
				return (FALSE);
			}

		}
		if (clienteDisponible(iph, &idx)) { // Si el cliente puede recibir comandos
			strcpy(tbsockets[idx].estado, CLIENTE_OCUPADO); // Actualiza el estado del cliente
			if (!mandaTrama(&tbsockets[idx].sock, ptrTrama)) {
				errorLog(modulo, 26, FALSE);
				return (FALSE);
			}
			//close(tbsockets[idx].sock); // Cierra el socket del cliente hasta nueva disponibilidad
		}
		tbl.MoveNext();
	}
	return (TRUE); // No existen registros
}
// ********************************************************************************************************
// PROGRAMA PRINCIPAL (SERVICIO)
// ********************************************************************************************************
int main(int argc, char *argv[]) {
	int i;
	SOCKET socket_s; // Socket donde escucha el servidor
	SOCKET socket_c; // Socket de los clientes que se conectan
	socklen_t iAddrSize;
	struct sockaddr_in local, cliente;
	char modulo[] = "main()";
	int activo=1;

	/*--------------------------------------------------------------------------------------------------------
	 Validación de parámetros de ejecución y lectura del fichero de configuración del servicio
	 ---------------------------------------------------------------------------------------------------------*/
	if (!validacionParametros(argc, argv, 1)) // Valida parámetros de ejecución
		exit(EXIT_FAILURE);

	if (!tomaConfiguracion(szPathFileCfg)) { // Toma parametros de configuracion
		exit(EXIT_FAILURE);
	}
	/*--------------------------------------------------------------------------------------------------------
	 Carga del catálogo de funciones que procesan las tramas (referencia directa por puntero a función)
	 ---------------------------------------------------------------------------------------------------------*/
	int cf = 0;

	strcpy(tbfuncionesServer[cf].nf, "Sondeo");
	tbfuncionesServer[cf++].fptr = &Sondeo;
	strcpy(tbfuncionesServer[cf].nf, "respuestaSondeo");
	tbfuncionesServer[cf++].fptr = &respuestaSondeo;

	strcpy(tbfuncionesServer[cf].nf, "ConsolaRemota");
	tbfuncionesServer[cf++].fptr = &ConsolaRemota;

	strcpy(tbfuncionesServer[cf].nf, "EcoConsola");
	tbfuncionesServer[cf++].fptr = &EcoConsola;

	strcpy(tbfuncionesServer[cf].nf, "Actualizar");
	tbfuncionesServer[cf++].fptr = &Actualizar;

	strcpy(tbfuncionesServer[cf].nf, "Purgar");
	tbfuncionesServer[cf++].fptr = &Purgar;

	strcpy(tbfuncionesServer[cf].nf, "InclusionCliente");
	tbfuncionesServer[cf++].fptr = &InclusionCliente;

	strcpy(tbfuncionesServer[cf].nf, "InclusionClienteWinLnx");
	tbfuncionesServer[cf++].fptr = &InclusionClienteWinLnx;

	strcpy(tbfuncionesServer[cf].nf, "AutoexecCliente");
	tbfuncionesServer[cf++].fptr = &AutoexecCliente;

	strcpy(tbfuncionesServer[cf].nf, "ComandosPendientes");
	tbfuncionesServer[cf++].fptr = &ComandosPendientes;

	strcpy(tbfuncionesServer[cf].nf, "DisponibilidadComandos");
	tbfuncionesServer[cf++].fptr = &DisponibilidadComandos;

	strcpy(tbfuncionesServer[cf].nf, "Arrancar");
	tbfuncionesServer[cf++].fptr = &Arrancar;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_Arrancar");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_Arrancar;

	strcpy(tbfuncionesServer[cf].nf, "Apagar");
	tbfuncionesServer[cf++].fptr = &Apagar;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_Apagar");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_Apagar;

	strcpy(tbfuncionesServer[cf].nf, "Reiniciar");
	tbfuncionesServer[cf++].fptr = &Reiniciar;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_Reiniciar");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_Reiniciar;

	strcpy(tbfuncionesServer[cf].nf, "IniciarSesion");
	tbfuncionesServer[cf++].fptr = &IniciarSesion;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_IniciarSesion");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_IniciarSesion;

	strcpy(tbfuncionesServer[cf].nf, "CrearImagen");
	tbfuncionesServer[cf++].fptr = &CrearImagen;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_CrearImagen");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_CrearImagen;

	strcpy(tbfuncionesServer[cf].nf, "CrearImagenBasica");
	tbfuncionesServer[cf++].fptr = &CrearImagenBasica;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_CrearImagenBasica");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_CrearImagenBasica;

	strcpy(tbfuncionesServer[cf].nf, "CrearSoftIncremental");
	tbfuncionesServer[cf++].fptr = &CrearSoftIncremental;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_CrearSoftIncremental");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_CrearSoftIncremental;

	strcpy(tbfuncionesServer[cf].nf, "RestaurarImagen");
	tbfuncionesServer[cf++].fptr = &RestaurarImagen;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_RestaurarImagen");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_RestaurarImagen;

	strcpy(tbfuncionesServer[cf].nf, "RestaurarImagenBasica");
	tbfuncionesServer[cf++].fptr = &RestaurarImagenBasica;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_RestaurarImagenBasica");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_RestaurarImagenBasica;

	strcpy(tbfuncionesServer[cf].nf, "RestaurarSoftIncremental");
	tbfuncionesServer[cf++].fptr = &RestaurarSoftIncremental;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_RestaurarSoftIncremental");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_RestaurarSoftIncremental;

	strcpy(tbfuncionesServer[cf].nf, "Configurar");
	tbfuncionesServer[cf++].fptr = &Configurar;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_Configurar");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_Configurar;

	strcpy(tbfuncionesServer[cf].nf, "EjecutarScript");
	tbfuncionesServer[cf++].fptr = &EjecutarScript;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_EjecutarScript");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_EjecutarScript;

	strcpy(tbfuncionesServer[cf].nf, "InventarioHardware");
	tbfuncionesServer[cf++].fptr = &InventarioHardware;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_InventarioHardware");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_InventarioHardware;

	strcpy(tbfuncionesServer[cf].nf, "InventarioSoftware");
	tbfuncionesServer[cf++].fptr = &InventarioSoftware;
	strcpy(tbfuncionesServer[cf].nf, "RESPUESTA_InventarioSoftware");
	tbfuncionesServer[cf++].fptr = &RESPUESTA_InventarioSoftware;

	strcpy(tbfuncionesServer[cf].nf, "enviaArchivo");
	tbfuncionesServer[cf++].fptr = &enviaArchivo;

	strcpy(tbfuncionesServer[cf].nf, "recibeArchivo");
	tbfuncionesServer[cf++].fptr = &recibeArchivo;

	strcpy(tbfuncionesServer[cf].nf, "envioProgramacion");
	tbfuncionesServer[cf++].fptr = &envioProgramacion;

	/*--------------------------------------------------------------------------------------------------------
	 // Inicializa array de información de los clientes
	 ---------------------------------------------------------------------------------------------------------*/
	for (i = 0; i < MAXIMOS_CLIENTES; i++) {
		tbsockets[i].ip[0] = '\0';
		tbsockets[i].sock = INVALID_SOCKET;
	}
	/*--------------------------------------------------------------------------------------------------------
	 Creación y configuración del socket del servicio
	 ---------------------------------------------------------------------------------------------------------*/
	socket_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Crea socket del servicio
	setsockopt(socket_s, SOL_SOCKET, SO_REUSEPORT, &activo, sizeof(int));
	if (socket_s == SOCKET_ERROR) { // Error al crear el socket del servicio
		errorLog(modulo, 13, TRUE);
		exit(EXIT_FAILURE);
	}

	local.sin_addr.s_addr = htonl(INADDR_ANY); // Configura el socket del servicio
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(puerto));

	if (bind(socket_s, (struct sockaddr *) &local, sizeof(local))
			== SOCKET_ERROR) { // Enlaza socket
		errorLog(modulo, 14, TRUE);
		exit(EXIT_FAILURE);
	}

	listen(socket_s, 250); // Pone a escuchar al socket
	iAddrSize = sizeof(cliente);
	/*--------------------------------------------------------------------------------------------------------
	 Bucle para acceptar conexiones
	 ---------------------------------------------------------------------------------------------------------*/
	infoLog(1); // Inicio de sesión
	while (TRUE) {
		socket_c = accept(socket_s, (struct sockaddr *) &cliente, &iAddrSize);
		if (socket_c == INVALID_SOCKET) {
			errorLog(modulo, 15, TRUE);
			exit(EXIT_FAILURE);
		}
		swcSocket = FALSE; // Por defecto se cerrara el socket de cliente después del anális de la trama
		if (!gestionaTrama(&socket_c)) {
			errorLog(modulo, 39, TRUE);
			//close(socket_c);/tmp/
			//break;
		}
		if (!swcSocket) // Sólo se cierra cuando el cliente NO espera comandos ineractivos
			close(socket_c);
	}
	/*--------------------------------------------------------------------------------------------------------
	 Fin del servicio
	 ---------------------------------------------------------------------------------------------------------*/
	close(socket_s);
	exit(EXIT_SUCCESS);
}
