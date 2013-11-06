<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Checkout form progress'),
//  'description' => t('Submit buttons for the checkout form.'),
  'required context' => new ctools_context_required(t('Commerce Checkout Page'), 'commerce_checkout_page'),
  'category' => t('Commerce'),
);

function commerce_panels_integration_commerce_checkout_progress_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  //$block->module = t('form');

  $block->title = '';
  //$block->delta = 'buttons';

  $pages = commerce_checkout_progress_get_items();
  $block->content = array(
    '#theme' => 'commerce_checkout_progress_list',
    '#items' => $pages,
    '#type' => variable_get('commerce_checkout_progress_list_type', 'ol'),
    '#current_page' => $context->data['page_id'],
    '#link' => variable_get('commerce_checkout_progress_link', TRUE),
  );
  return $block;
}

function commerce_panels_integration_commerce_checkout_progress_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" checkout form submit buttons', array('@s' => $context->identifier));
}

function commerce_panels_integration_commerce_checkout_progress_content_type_edit_form($form, &$form_state) {
  // provide a blank form so we have a place to have context setting.
  return $form;
}

