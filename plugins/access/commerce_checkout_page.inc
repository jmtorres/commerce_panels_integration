<?php

$plugin = array(
  'title' => t("Commerce Checkout Page"),
  'description' => t('Control access by length of commerce checkout page.'),
  'callback' => 'commerce_checkout_page_access_check',
  'settings form' => 'commerce_checkout_page_access_settings',
  'summary' => 'commerce_checkout_page_access_summary',
  'required context' => new ctools_context_required(t('Commerce Checkout Page'), 'commerce_checkout_page'),
);


function commerce_checkout_page_access_settings($form, &$form_state, $conf) {
  $checkout_pages = commerce_checkout_pages();

  $form['settings']['checkout_page'] = array(
    '#title' => t('Commerce Checkout Page'),
    '#type' => 'select', 
    '#options' => drupal_map_assoc(array_keys($checkout_pages)), 
    '#default_value' => $conf['checkout_page'],   
  );

  return $form;
}

function commerce_checkout_page_access_check($conf, $context) {
  if (empty($context) || empty($context->data)) {
    return FALSE;
  }
  if ($context->data['page_id'] == $conf['checkout_page']) {
    return TRUE;
  }
  return FALSE;
}

function commerce_checkout_page_access_summary($conf, $context) {
  $checkout_page = commerce_checkout_page_load($conf['checkout_page']);
  return t("@identifier is '@checkout_page'", array(
    '@identifier' => $context->identifier,
    '@checkout_page' => $checkout_page['title'],
  ));
}
