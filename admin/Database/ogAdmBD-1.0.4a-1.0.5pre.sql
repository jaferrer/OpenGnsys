### Fichero de actualización de la base de datos.
#use ogAdmBD
# Añadir validación del cliente.
ALTER TABLE aulas
	ADD validacion TINYINT(1) DEFAULT 0,
	ADD paginalogin VARCHAR(100),
	ADD paginavalidacion VARCHAR(100);

ALTER TABLE ordenadores
	ADD validacion TINYINT(1) DEFAULT 0,
	ADD paginalogin VARCHAR(100),
	ADD paginavalidacion VARCHAR(100);

# Nuevos comandos.
ALTER TABLE comandos
	ADD submenu VARCHAR(50) NOT NULL DEFAULT '';
INSERT INTO comandos (idcomando, descripcion, pagina, gestor, funcion, urlimg, aplicambito, visuparametros, parametros, comentarios, activo, submenu) VALUES
	(11, 'Eliminar Imagen Cache', '../comandos/EliminarImagenCache.php', '../comandos/gestores/gestor_Comandos.php', 'EliminarImagenCache', '', 31, 'iph;tis;dcr;scp', 'nfn;iph;tis;dcr;scp', '', 1, ''),
	(12, 'Crear Imagen Basica', '../comandos/CrearImagenBasica.php', '../comandos/gestores/gestor_Comandos.php', 'CrearImagenBasica', '', 16, 'dsk;par;cpt;idi;nci;ipr;iph;bpi;cpc;bpc;rti;nba', 'nfn;dsk;par;cpt;idi;nci;ipr;iph;bpi;cpc;bpc;rti;nba', '', 1, 'Sincronizacion'),
	(13, 'Restaurar Imagen Basica', '../comandos/RestaurarImagenBasica.php', '../comandos/gestores/gestor_Comandos.php', 'RestaurarImagenBasica', '', 28, 'dsk;par;idi;nci;ipr;iph;bpi;cpc;bpc;rti;nba;met', 'nfn;dsk;par;idi;nci;ipr;iph;bpi;cpc;bpc;rti;nba;met', '', 1, 'Sincronizacion'),
	(14, 'Crear Software Incremental', '../comandos/CrearSoftIncremental.php', '../comandos/gestores/gestor_Comandos.php', 'CrearSoftIncremental', '', 16, 'dsk;par;idi;nci;ipr;idf;ncf;bpi;cpc;bpc;iph;rti;nba', 'nfn;dsk;par;idi;nci;ipr;idf;ncf;bpi;cpc;bpc;iph;rti;nba', '', 1, 'Sincronizacion'),
	(15, 'Restaurar Software Incremental', '../comandos/RestaurarSoftIncremental.php', '../comandos/gestores/gestor_Comandos.php', 'RestaurarSoftIncremental', '', 28, 'dsk;par;idi;nci;ipr;idf;ncf;bpi;cpc;bpc;iph;rti;met;nba', 'nfn;dsk;par;idi;nci;ipr;idf;ncf;bpi;cpc;bpc;iph;rti;met;nba', '', 1, 'Sincronizacion');

# Parámetros para los comandos nuevos.
ALTER TABLE parametros
	ADD KEY (nemonico);
INSERT INTO parametros (idparametro, nemonico, descripcion, nomidentificador, nomtabla, nomliteral, tipopa, visual) VALUES
	(31, 'idf', 'Imagen Incremental', 'idimagen', 'imagenes', 'descripcion', 1, 1),
	(32, 'ncf', 'Nombre canónico de la Imagen Incremental', '', '', '', 0, 1),
	(33, 'bpi', 'Borrar imagen o partición previamente', '', '', '', 5, 1),
	(34, 'cpc', 'Copiar también en cache', '', '', '', 5, 1),
	(35, 'bpc', 'Borrado previo de la imagen en cache', '', '', '', 5, 1),
	(36, 'rti', 'Ruta de origen', '', '', '', 0, 1),
	(37, 'met', 'Método clonación', ';', '', 'Desde caché; Desde repositorio', 3, 1),
	(38, 'nba', 'No borrar archivos en destino', '', '', '', 0, 1);

# Imágenes incrementales.
ALTER TABLE imagenes
	ADD tipo TINYINT NULL,
	ADD imagenid INT NOT NULL DEFAULT '0',
	ADD ruta VARCHAR(250) NULL;
UPDATE imagenes SET tipo=1;

# Cambio de tipo de grupo.
UPDATE grupos SET tipo=70 WHERE tipo=50;
