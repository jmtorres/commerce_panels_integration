<?php

/**
 * @file
 *
 * Plugin to provide a vocabulary context
 */
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
    'title' => t("Commerce Checkout Context"),
    'description' => t('Context required for the commerce checkout page'),
    'context' => 'commerce_panels_integration_create_commerce',
    'keyword' => 'commerce_order',
    'context name' => 'commerce',
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function commerce_panels_integration_create_commerce($empty, $data = NULL, $conf = FALSE) {

  $context = new ctools_context('commerce');
  $context->plugin = 'commerce';

  if ($empty) {
    return $context;
  }

  if (!empty($data)) {
    $context->data = $data;

    return $context;
  }
}

