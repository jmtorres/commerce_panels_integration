<?php

/**
 * @file
 * Commerce panels integration plugin to handle the checkout form.
 */

 /**
  * Plugins are described by creating a $plugin array which will be used
  * by the system that includes this file.
  */

$plugin = array(
  'single' => TRUE,
  'title' => t('Commerce Checkout Form'),
  'description' => t('The new checkout form'),
  'category' => t('Widgets'),
);

/**
 * Load the commerce checkout pages to be able to call the function
 * commerce_cehckout_router()
 */

module_load_include('inc', 'commerce_checkout', 'includes/commerce_checkout.pages');

/**
 * Output function for the commerce checkout form.
 */
function commerce_panels_integration_commerce_checkout_form_content_type_render($subtype, $conf, $panel_args, $context) {

  $order = isset($panel_args) ? clone($panel_args[0]) : NULL;
  $checkout_page = isset($panel_args) ? $panel_args[1] : NULL;

  $block = new stdClass();

  $block->content = commerce_checkout_router($order, $checkout_page);

  return $block;
}

/**
 * A blank form so we have a place to have context settings
 */
function commerce_panels_integration_commerce_checkout_form_content_type_edit_form($form, &$form_state) {
  // Provide a blank form so we have a place to have context setting.
  return $form;
}
