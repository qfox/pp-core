#!/usr/bin/env php5
<?php

// bootstrap
set_time_limit(0);
require_once dirname(__FILE__).'/../../libpp/lib/maincommon.inc';
if (file_exists($localcommon = BASEPATH.'local/lib/maincommon.inc')) {
	require_once ($localcommon);
}

ini_set('display_errors', '1');
$engine = new PXEngineSbin();

$app = PXRegistry::getApp();
$db = PXRegistry::getDb();



// defaults
$limit = 100;
$skipTypes = array(
	'suser',
	'sgroup',
	'sys_regions',
	'adbanner',
	'adplace',
	'adcampaign'
);

$export = array();

// fetch common data
foreach ($app->types as $typeKey => $type) {
	if (in_array($type->id, $skipTypes)) continue;

	$parentType = $type->parent? $type->parentType() : null;
	if (!is_object($parentType)) {
//		d20($type);die;
	}
	if ($parentType && in_array($parentType->id, $skipTypes)) continue;

	// fetch all data for type
	$export['data'][$type->id] = $db->getObjects($type, null);

	// fetch schema data - todo.
	// $export['schema']
}

// fetch references
/*foreach ($app->references as $k => $v) {
	if (in_array($v['from'], $skipTypes) || in_array($v['to'], $skipTypes)) continue;
	// todo: make it if you need it
}*/

// fetch file data
$outpath = tempnam(BASEPATH.'/tmp', 'pp.export');
unlink($outpath);
MakeDirIfNotExists($outpath);
foreach ($export['data'] as $typeKey => $objects) {
	$type = $app->types[$typeKey];
	foreach ($type->fields as $fk => $field) {
		if (! $field->storageType instanceof PXStorageTypeFile) continue;
		foreach ($objects as $id => &$object) {
			if (empty($object[$fk])) continue;
			$file = HTDOCS_PATH . $object[$fk]['path'];
			if (!file_exists($file)) { // clean if no file. broken data
				$objects[$id][$fk] = null;
			} else {
				$dest = $outpath . $object[$fk]['path'];
				MakeDirIfNotExists(dirname($dest));
				copy($file, $dest);
			}
		}
	}
}
$aifilestar = $outpath . '/ppdata.files.tar.gz';

// fetch modules list

// fetch modules code - todo

// fetch modules descriptions - todo



// dump
`cd $outpath; tar -cvzf $aifilestar ./; rm -r $outpath/ai`;
echo 'files: ' . $aifilestar . PHP_EOL;

$outdata = json_encode_koi($export);
$outfile = $outpath . '/ppdata.json';
file_put_contents($outfile, $outdata); // or dump to stdout
echo 'data json: ' . $outfile . PHP_EOL;
//echo $outdata;

