<?php

$plugin = array(
  'title' => t('Commerce Checkout Page'),
  'keyword' => 'commerce_checkout_page',
  'description' => t('Creates a commerce_checkout_page context from a commerce_checkout_page argument.'),
  'context' => 'commerce_panels_integration_argument_commerce_checkout_page_context',
  'path placeholder' => '%pm_arg',
);

dpm(get_defined_vars());

function commerce_panels_integration_argument_commerce_checkout_page_context($arg = NULL, $conf = NULL, $empty = FALSE) {
dpm(get_defined_vars());
  if($empty) {
    return ctools_context_create_empty('commerce_checkout_page');
  }

  if (is_object($arg)) {
    return ctools_context_create('commerce_checkout_page', $arg);
  }

  $checkout_page = commerce_checkout_page_load($arg);

  if(!$checkout_page) {
    return FALSE;
  }

  return ctools_context_create('commerce_checkout_page', $checkout_page);
}
