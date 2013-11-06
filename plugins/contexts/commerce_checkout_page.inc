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
  $context = new ctools_context(array('form', 'commerce_checkout_page'));
  $context->plugin = 'commerce_checkout_page';

  if ($empty) {
    return $context;
  }

  if (!empty($data)) {
    $context->data = $data;
    return $context;
  }
}
