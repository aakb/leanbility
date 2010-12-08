<?php
// $Id$

class LeanbilityServerProvider {

  private $borr_card = NULL;
  private static $conf = NULL;
  private static $status_values = NULL;

  // Default return value
  private $default_status = 'none';

  public function  __construct($borr_card) {
    $this->borr_card = $borr_card;
  }
  
  public function status() {
    // Default return value
    $ret = $this->default_status;

    // Load configuration
    $this->loadConf();
    
    // Check cache
    $cache = cache_get($this->borr_card, 'cache_leanbility');
    if ($cache) {
     // If found in cache, return status
     if ($cache->data[LEANBILITY_SUB]) {
        $ret = $cache->data[LEANBILITY_SUB_TYPE];
     }
    }
    else {
      // If not found in cache load configuration an request the service for status      
      $request = drupal_http_request($this->build_url());

      if ($request->code != 200) {
        throw new Exception('Leanbility server did not answer correctly.', $request->code);
      }
      else {
        // Parse answer and cache it
        $status = $this->parse_status($request->data);
        cache_set($this->borr_card, $status, time() + self::$conf['cache_duration']);
        if ($status[LEANBILITY_SUB]) {
          $ret = $status[LEANBILITY_SUB_TYPE];
        }
        else {
          // Not subscribed
          $ret = $this->default_status;
        }
      }
    }

    return $ret;
  }

  public function update($type) {
    // Load configuration
    $this->loadConf();

    // Build request
    $url = $this->build_url();
    if ($type == $this->default_status) {
       $url .= '&signup=false&type=mobile'; // type is required, so just send something
    }
    else {
      $url .= '&signup=true&type=' . $type;
    }

    // Load configuration
    $this->loadConf();

    // Send request to the server
    $request = drupal_http_request($url);

    // Update cache on success
    $status = $this->parse_status($request->data);
    cache_set($this->borr_card, $status, time() + self::$conf['cache_duration']);
  }
  
  private function parse_status($xml) {
    $ret = array();
    $sxml = new SimpleXMLElement($xml);
    if ($sxml->status[0]->responsecode == 200) {
      $ret[LEANBILITY_SUB] = TRUE;
      if ($sxml->signup[0]->accepted == 'False') {
        $ret[LEANBILITY_SUB] = FALSE;
      }
      $ret[LEANBILITY_SUB_TYPE] = strtolower((string)$sxml->signup[0]->type);
    }
    else {
      throw new Exception((string)$sxml->status[0]->message, (int)$sxml->status[0]->responsecode);
    }
    return $ret;
  }

  private function build_url() {
    // Load conf
    $this->loadConf();
    
    // Build string
    return self::$conf['service_url'] . '?secureid=' . self::$conf['secure_token'] . '&tickno=' . $this->borr_card;
  }

  private function loadConf() {
    if (self::$conf == NULL) {      
      self::$conf = variable_get('leanbility', array());
      self::$status_values = array();
      foreach (self::$conf['service_types'] as $key => $value) {
        self::$status_values[$key] = t(ucfirst($value));
      }
    }
  }
}
