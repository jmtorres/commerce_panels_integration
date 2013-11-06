<?php

$plugin = array(
  'title' => t('Commerce Checkout Page'),
  'keyword' => 'commerce_checkout_page',
  'description' => t('Creates a commerce_checkout_page context from a commerce_checkout_page argument.'),
  'context' => 'commerce_panels_integration_argument_commerce_checkout_page_context',
  'path placeholder' => '%pm_arg',
);

function commerce_panels_integration_argument_commerce_checkout_page_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('commerce_checkout_page');
  }

  if (empty($arg)) {
    $arg = commerce_checkout_first_checkout_page();
  }

  if (is_array($arg)) {
    return ctools_context_create('commerce_checkout_page', $arg);
  }

  $checkout_page = commerce_checkout_page_load($arg);

  if(!$checkout_page) {
    return ctools_context_create_empty('commerce_checkout_page');
  }

  return ctools_context_create('commerce_checkout_page', $checkout_page);
}
