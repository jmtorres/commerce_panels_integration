<?php

/**
 * @file
 *
 * Plugin to provide a commerce_checkout_page context
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Commerce Checkout Page"),
  'description' => t('Context required for the commerce checkout page'),
  'context' => 'commerce_panels_integration_create_commerce_checkout_page',
  'keyword' => 'commerce_checkout_page',
  'context name' => 'commerce_checkout_page',
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function commerce_panels_integration_create_commerce_checkout_page($empty, $data = NULL, $conf = FALSE) {
dpm("I WAS CALLED!!");
  $context = new ctools_context('commerce_checkout_page');
  $context->plugin = 'commerce_checkout_page';
dpm(get_defined_vars());
  if ($empty) {
    $checkout_page = commerce_checkout_first_checkout_page();
  }
  else {
    $page_id = is_array($data) && isset($data['page_id']) ? $data['page_id'] : NULL;

    if (is_array($data)) {
      $data = commerce_checkout_page_load($page_id);
    }
  }

  if (!empty($data)) {
    $context->data = $data;

    return $context;
  }
}
