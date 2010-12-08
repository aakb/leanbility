<?php
// $Id$

/**
 * Implementation of the administation form
 *
 * @param array $form_state
 * @return array
 */
function leanbility_admin_form(&$form_state) {

  $form = array(
    '#tree' => TRUE,
  );

  $defaults = variable_get('leanbility', array());
  $form['leanbility']['service_url'] = array(
    '#type' => 'textfield',
    '#title' => 'Service URL',
    '#required' => TRUE,
    '#default_value' => isset($defaults['service_url']) ? $defaults['service_url'] : '',
    '#description' => t('The complate url to the leanbility services aspx page (e.g http://reservationqueue.aakb.dk/Service/Tilmelding.aspx).'),
  );

  $form['leanbility']['secure_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Secure token (id)'),
    '#required' => TRUE,
    '#default_value' => isset($defaults['secure_token']) ? $defaults['secure_token'] : '',
  );

  $options = array('mobile' => t('Mobile'), 'email' => t('Email'), 'none' => t('None'));
  $form['leanbility']['service_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Activate services'),
    '#required' => TRUE,
    '#options' => $options,
    '#default_value' => isset($defaults['service_types']) ? $defaults['service_types'] : array(),
  );

  $form['leanbility']['cache_duration'] = array(
    '#type' => 'select',
    '#title' => t('Cache duration'),
    '#description' => t('Determines how long leanbility status is cached.'),
    '#options' => array(
      0 => t('No cacheing'),
      300 => t('5 minutes'),
      600 => t('10 minutes'),
      1200 => t('20 minutes'),
      1800 => t('30 minutes'),
      2400 => t('40 minutes'),
      3600 => t('60 minutes'),
    ),
    '#default_value' => isset($defaults['cache_duration']) ? $defaults['cache_duration'] : 600,
  );

  return system_settings_form($form);
}
