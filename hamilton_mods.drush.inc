<?php

/**
 * @file
 * A drush script for updating Hamilton's MODS records.
 */

/**
 * Implements hook_drush_command().
 */
function hamilton_mods_drush_command() {
  $items = array();
  $items['hamilton_process_mods'] = array(
    'callback' => 'hamilton_mods_process_mods',
    'description' => 'Update the MODS records given a key value array.',
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com hamilton_apply_mods',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['hamilton_update_labels'] = array(
    'callback' => 'hamilton_mods_update_labels',
    'description' => 'Update the labels of the Hamilton objects.',
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com hamilton_update_labels',
    ),
  );
  $items['hamilton_fix_moved'] = array(
    'callback' => 'hamilton_mods_fix_relationships',
    'description' => 'Remove errorneous RELS-EXT relationships.',
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'simple example' => 'drush -u 1 -l http://example.com hamilton_fix_moved',
    ),
  );
  return $items;
}

function hamilton_mods_apply_mods($pid, $file_loc) {
  if (file_exists($file_loc)) {
    $object = islandora_object_load($pid);
    if (strpos($file_loc, 'tran') != FALSE) {
      $tran_ds = $object->constructDatastream('TRANMODS', 'X');
      $tran_ds->label = 'Transcription MODS Record';
      $tran_ds->mimetype = 'text/xml';
      $tran_ds->setContentFromFile($file_loc);
      $object->ingestDatastream($tran_ds);
    }
    else {
      $mods_ds = $object->constructDatastream('MODS', 'X');
      $mods_ds->label = 'MODS Record';
      $mods_ds->mimetype = 'text/xml';
      $mods_ds->setContentFromFile($file_loc);
      $object->ingestDatastream($mods_ds);

      $dc_ds = $object['DC'];
      $xslt = new XSLTProcessor();
      $xsl = new DOMDocument();
      $xsl->load(drupal_get_path('module', 'hamilton_mods') . '/mods_to_dc.xsl');

      $input = new DomDocument();
      $did_load_ok = $input->loadXML($mods_ds->content);
      if ($did_load_ok) {
        $xslt->importStylesheet($xsl);
        $dc_output = $xslt->transformToXml($input);
        $dc_ds->content = $dc_output;
        watchdog('Hamilton MODS', 'Updated the MODS and DC for @pid', array('@pid' => $pid), WATCHDOG_INFO);
      }
      else {
        drush_set_error("Bad MODS", "Unable to load the MODS from $pid", "Error: ");
        return;
      }
    }
  }
  else {
    drush_set_error("Invalid file location passed", "$file_loc does not point to a valid file", "Error: ");
    return;
  }
}

function hamilton_mods_process_mods() {
  $mods_array = array(
    "shahid-cp010_MODS.xml" => "hamilton:113",
    "shahid-ess001-001-tran_MODS.xml" => "hamilton:6",
    "shahid-ess001-001_MODS.xml" => "hamilton:6",
    "shahid-ess001-002-tran_MODS.xml" => "hamilton:14",
    "shahid-ess001-002_MODS.xml" => "hamilton:14",
    "shahid-ess001-003-tran_MODS.xml" => "hamilton:12",
    "shahid-ess001-003_MODS.xml" => "hamilton:12",
    "shahid-ess001-004-tran_MODS.xml" => "hamilton:11",
    "shahid-ess001-004_MODS.xml" => "hamilton:11",
    "shahid-ess001-005-tran_MODS.xml" => "hamilton:7",
    "shahid-ess001-005_MODS.xml" => "hamilton:7",
    "shahid-ess001-006-tran_MODS.xml" => "hamilton:15",
    "shahid-ess001-006_MODS.xml" => "hamilton:15",
    "shahid-ess001-007-tran_MODS.xml" => "hamilton:8",
    "shahid-ess001-007_MODS.xml" => "hamilton:8",
    "shahid-ess001-008-tran_MODS.xml" => "hamilton:9",
    "shahid-ess001-008_MODS.xml" => "hamilton:9",
    "shahid-ess001-009-tran_MODS.xml" => "hamilton:13",
    "shahid-ess001-009_MODS.xml" => "hamilton:13",
    "shahid-ess001-010-tran_MODS.xml" => "hamilton:10",
    "shahid-ess001-010_MODS.xml" => "hamilton:10",
    "shahid-ess001-tran_MODS.xml" => "hamilton:5",
    "shahid-ess001_MODS.xml" => "hamilton:5",
    "shahid-let008-001-tran_MODS.xml" => "hamilton:101",
    "shahid-let008-001_MODS.xml" => "hamilton:101",
    "shahid-let008-002-tran_MODS.xml" => "hamilton:102",
    "shahid-let008-002_MODS.xml" => "hamilton:102",
    "shahid-let008-tran_MODS.xml" => "hamilton:56",
    "shahid-let008_MODS.xml" => "hamilton:56",
    "shahid-let010-001_MODS.xml" => "hamilton:74",
    "shahid-let010-002_MODS.xml" => "hamilton:75",
    "shahid-let010-tran_MODS.xml" => "hamilton:73",
    "shahid-let010_MODS.xml" => "hamilton:73",
    "shahid-man001-001-tran_MODS.xml" => "hamilton:42",
    "shahid-man001-001_MODS.xml" => "hamilton:42",
    "shahid-man001-002-tran_MODS.xml" => "hamilton:43",
    "shahid-man001-002_MODS.xml" => "hamilton:43",
    "shahid-man001-tran_MODS.xml" => "hamilton:41",
    "shahid-man001_MODS.xml" => "hamilton:41",
    "shahid-man007-001-tran_MODS.xml" => "hamilton:93",
    "shahid-man007-001_MODS.xml" => "hamilton:93",
    "shahid-man007-002-001-tran_MODS.xml" => "hamilton:91",
    "shahid-man007-002-001_MODS.xml" => "hamilton:91",
    "shahid-man007-002-002-tran_MODS.xml" => "hamilton:92",
    "shahid-man007-002-002_MODS.xml" => "hamilton:92",
    "shahid-man007-003-tran_MODS.xml" => "hamilton:103",
    "shahid-man007-003_MODS.xml" => "hamilton:103",
    "shahid-man007-004-001-tran_MODS.xml" => "hamilton:104",
    "shahid-man007-004-001_MODS.xml" => "hamilton:104",
    "shahid-man007-004-002-tran_MODS.xml" => "hamilton:105",
    "shahid-man007-004-002_MODS.xml" => "hamilton:105",
    "shahid-man007-005-tran_MODS.xml" => "hamilton:106",
    "shahid-man007-005_MODS.xml" => "hamilton:106",
    "shahid-man007-tran_MODS.xml" => "hamilton:90",
    "shahid-man007_MODS.xml" => "hamilton:90",
    "shahid-man009-tran_MODS.xml" => "hamilton:22",
    "shahid-man009-ver001-tran---review required_MODS.xml" => "hamilton:24",
    "shahid-man009-ver001_MODS.xml" => "hamilton:24",
    "shahid-man009-ver002-tran_MODS.xml" => "hamilton:100",
    "shahid-man009-ver002_MODS.xml" => "hamilton:100",
    "shahid-man009-ver003-tran_MODS.xml" => "hamilton:31",
    "shahid-man009-ver003_MODS.xml" => "hamilton:31",
    "shahid-man009-ver004-tran_MODS.xml" => "hamilton:26",
    "shahid-man009-ver004_MODS.xml" => "hamilton:26",
    "shahid-man009-ver005-tran_MODS.xml" => "hamilton:28",
    "shahid-man009-ver005_MODS.xml" => "hamilton:28",
    "shahid-man009-ver006-tran_MODS.xml" => "hamilton:30",
    "shahid-man009-ver006_MODS.xml" => "hamilton:30",
    "shahid-man009-ver007-tran_MODS.xml" => "hamilton:29",
    "shahid-man009-ver007_MODS.xml" => "hamilton:29",
    "shahid-man009-ver008-tran_MODS.xml" => "hamilton:25",
    "shahid-man009-ver008_MODS.xml" => "hamilton:25",
    "shahid-man009_MODS.xml" => "hamilton:22",
    "shahid-man010_MODS.xml" => "hamilton:68",
    "shahid-man010-001-tran_MODS.xml" => "hamilton:69",
    "shahid-man010-001_MODS.xml" => "hamilton:69",
    "shahid-man011-001-tran_MODS.xml" => "hamilton:96",
    "shahid-man011-001_MODS.xml" => "hamilton:96",
    "shahid-man046-001-tran_MODS.xml" => "hamilton:78",
    "shahid-man046-001_MODS.xml" => "hamilton:78",
    "shahid-mis001-001-tran_MODS.xml" => "hamilton:51",
    "shahid-mis001-001_MODS.xml" => "hamilton:51",
    "shahid-mis001-001-tran_MODS.xml" => "hamilton:52",
    "shahid-mis001-001_MODS.xml" => "hamilton:52",
    "shahid-mis001-002-tran_MODS.xml" => "hamilton:53",
    "shahid-mis001-002_MODS.xml" => "hamilton:53",
    "shahid-mis001-002-tran_MODS.xml" => "hamilton:55",
    "shahid-mis001-002_MODS.xml" => "hamilton:55",
    "shahid-mis001-003-tran_MODS.xml" => "hamilton:49",
    "shahid-mis001-003_MODS.xml" => "hamilton:49",
    "shahid-mis001-004-tran_MODS.xml" => "hamilton:48",
    "shahid-mis001-004_MODS.xml" => "hamilton:48",
    "shahid-mis001-005-tran_MODS.xml" => "hamilton:50",
    "shahid-mis001-005_MODS.xml" => "hamilton:50",
    "shahid-mis001-006-tran_MODS.xml" => "hamilton:47",
    "shahid-mis001-006_MODS.xml" => "hamilton:47",
    "shahid-mis001-tran_MODS.xml" => "hamilton:46",
    "shahid-mis001_MODS.xml" => "hamilton:46",
    "shahid-pho002_MODS.xml" => "hamilton:110",
    "shahid-pho003_MODS.xml" => "hamilton:109",
    "shahid-pho004_MODS.xml" => "hamilton:108",
    "shahid-pho005_MODS.xml" => "hamilton:107",
    "shahid-poe007-001-tran_MODS.xml" => "hamilton:44",
    "shahid-poe007-001_MODS.xml" => "hamilton:44",
    "shahid-poe008-001_MODS.xml" => "hamilton:63",
    "shahid-poe008-002_MODS.xml" => "hamilton:62",
    "shahid-poe008-003_MODS.xml" => "hamilton:61",
    "shahid-poe008-004_MODS.xml" => "hamilton:60",
    "shahid-poe008-005_MODS.xml" => "hamilton:64",
    "shahid-poe008-tran_MODS.xml" => "hamilton:59",
    "shahid-poe008_MODS.xml" => "hamilton:59",
    "shahid-poe010-001-tran_MODS.xml" => "hamilton:76",
    "shahid-poe010-001_MODS.xml" => "hamilton:76",
    "shahid-poe011-001_MODS.xml" => "hamilton:82",
    "shahid-poe011-002_MODS.xml" => "hamilton:81",
    "shahid-poe011-tran_MODS.xml" => "hamilton:80",
    "shahid-poe011_MODS.xml" => "hamilton:80",
    "shahid-poe012-001_MODS.xml" => "hamilton:71",
    "shahid-poe012-002_MODS.xml" => "hamilton:72",
    "shahid-poe012-tran_MODS.xml" => "hamilton:70",
    "shahid-poe012_MODS.xml" => "hamilton:70",
    "shahid-poe013-001_MODS.xml" => "hamilton:84",
    "shahid-poe013-002_MODS.xml" => "hamilton:85",
    "shahid-poe013-tran_MODS.xml" => "hamilton:83",
    "shahid-poe013_MODS.xml" => "hamilton:83",
    "shahid-poe014-001-tran_MODS.xml" => "hamilton:3",
    "shahid-poe014-001_MODS.xml" => "hamilton:3",
  );
  $test_array = array(
    "shahid-poe014-001_MODS.xml" => "hamilton:3",
    "shahid-poe014-001-tran_MODS.xml" => "hamilton:3",
  );
  foreach ($mods_array as $filename => $pid) {
    $file_loc = "public://hamilton_mods/$filename";
    hamilton_mods_apply_mods($pid, $file_loc);
  }
}

function hamilton_mods_update_labels() {
  $pid_array = array(
    "hamilton:113",
    "hamilton:6",
    "hamilton:6",
    "hamilton:14",
    "hamilton:14",
    "hamilton:12",
    "hamilton:12",
    "hamilton:11",
    "hamilton:11",
    "hamilton:7",
    "hamilton:7",
    "hamilton:15",
    "hamilton:15",
    "hamilton:8",
    "hamilton:8",
    "hamilton:9",
    "hamilton:9",
    "hamilton:13",
    "hamilton:13",
    "hamilton:10",
    "hamilton:10",
    "hamilton:5",
    "hamilton:5",
    "hamilton:101",
    "hamilton:101",
    "hamilton:102",
    "hamilton:102",
    "hamilton:56",
    "hamilton:56",
    "hamilton:74",
    "hamilton:75",
    "hamilton:73",
    "hamilton:73",
    "hamilton:42",
    "hamilton:42",
    "hamilton:43",
    "hamilton:43",
    "hamilton:41",
    "hamilton:41",
    "hamilton:93",
    "hamilton:93",
    "hamilton:91",
    "hamilton:91",
    "hamilton:92",
    "hamilton:92",
    "hamilton:103",
    "hamilton:103",
    "hamilton:104",
    "hamilton:104",
    "hamilton:105",
    "hamilton:105",
    "hamilton:106",
    "hamilton:106",
    "hamilton:90",
    "hamilton:90",
    "hamilton:22",
    "hamilton:24",
    "hamilton:24",
    "hamilton:100",
    "hamilton:100",
    "hamilton:31",
    "hamilton:31",
    "hamilton:26",
    "hamilton:26",
    "hamilton:28",
    "hamilton:28",
    "hamilton:30",
    "hamilton:30",
    "hamilton:29",
    "hamilton:29",
    "hamilton:25",
    "hamilton:25",
    "hamilton:22",
    "hamilton:68",
    "hamilton:69",
    "hamilton:69",
    "hamilton:96",
    "hamilton:96",
    "hamilton:78",
    "hamilton:78",
    "hamilton:51",
    "hamilton:51",
    "hamilton:52",
    "hamilton:52",
    "hamilton:53",
    "hamilton:53",
    "hamilton:55",
    "hamilton:55",
    "hamilton:49",
    "hamilton:49",
    "hamilton:48",
    "hamilton:48",
    "hamilton:50",
    "hamilton:50",
    "hamilton:47",
    "hamilton:47",
    "hamilton:46",
    "hamilton:46",
    "hamilton:110",
    "hamilton:109",
    "hamilton:108",
    "hamilton:107",
    "hamilton:44",
    "hamilton:44",
    "hamilton:63",
    "hamilton:62",
    "hamilton:61",
    "hamilton:60",
    "hamilton:64",
    "hamilton:59",
    "hamilton:59",
    "hamilton:76",
    "hamilton:76",
    "hamilton:82",
    "hamilton:81",
    "hamilton:80",
    "hamilton:80",
    "hamilton:71",
    "hamilton:72",
    "hamilton:70",
    "hamilton:70",
    "hamilton:84",
    "hamilton:85",
    "hamilton:83",
    "hamilton:83",
    "hamilton:3",
    "hamilton:3",
  );
  $pid_array = array_unique($pid_array);
  foreach ($pid_array as $pid) {
    $object = islandora_object_load($pid);

    $dc = $object['DC']->content;
    $dc_xml = new SimpleXMLElement($dc);

    $title_result = $dc_xml->xpath('//dc:title');
    $title = reset($title_result);
    if ($title) {
      $object->label = (string) $title;
    }
  }
}

function hamilton_mods_fix_relationships() {
  $connection = islandora_get_tuque_connection();
  $query = 'select $object from <#ri> where
    $object <fedora-rels-ext:isMemberOf> <info:fedora/HamiltonCivilWar:UnitedStatesCivilWarLetters>
    and
    (
    $object <fedora-rels-ext:isMemberOfCollection> <info:fedora/islandora:audio_collection>
    or $object <fedora-rels-ext:isMemberOfCollection> <info:fedora/hamilton:shahid>
    )';
  $results = $connection->repository->ri->itqlQuery($query);

  foreach ($results as $result) {
    $object = islandora_object_load($result['object']['value']);
    dd($object->relationships->get());
  }
}