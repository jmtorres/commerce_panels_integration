<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Commerce Checkout Pane'),
  'description' => t('A Pane from the Commerce Checkout workflow.'),
  'required context' => array(
    new ctools_context_required(t('Commerce Order'), 'commerce_order'),
    new ctools_context_required(t('Commerce Checkout Page'), 'commerce_checkout_page'),
  ),
  'defaults' => array('pane' => ''),
  'category' => t('Commerce'),
  'css' => array(
    drupal_get_path('module', 'commerce_checkout') . '/theme/commerce_checkout.base.css',
    drupal_get_path('module', 'commerce_checkout') . '/theme/commerce_checkout.theme.css',
  ),
  'js' => array(drupal_get_path('module', 'commerce_checkout') . '/commerce_checkout.js'),
);


function commerce_panels_integration_commerce_checkout_pane_content_type_render($subtype, $conf, $panel_args, $context){
  $block = new stdClass();
  $pane_id = $conf['pane'];
  $block->delta = $pane_id;

  if (isset($context[1]->form)) {
    if (!empty($context[1]->form[$pane_id])) {
      $block->content[$pane_id] = $context[1]->form[$pane_id];
      $block->title = $context[1]->form[$pane_id]['#title'];
      unset($context[1]->form[$pane_id]);
    }
  }
  else {
    $block->content = t($pane_id . ' form');
  }
  return $block;
}

function commerce_panels_integration_commerce_checkout_pane_content_type_edit_form($form, &$form_state){
  $conf = $form_state['conf'];

  $checkout_pages = commerce_checkout_pages();

  $checkout_pages['disabled'] = array(
    'page_id' => 'disabled',
    'title' => t('Disabled'),
  );

  $panes = commerce_checkout_panes();

  $options = array();

  foreach ($panes as $pane_id => $checkout_pane) {
    $page_id = $checkout_pane['enabled'] ? $checkout_pane['page'] : 'disabled';

    // If the page no longer exists, place the pane on the first page.
    if (empty($checkout_pages[$page_id])) {
      reset($checkout_pages);
      $page_id = key($checkout_pages);
    }

    $options[$checkout_pages[$page_id]['title']][$pane_id] = check_plain($checkout_pane['name']);
  }

  $form['pane'] = array(
    '#title' => t('Checkout Pane'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['pane'],
  );

  return $form;
}


function commerce_panels_integration_commerce_checkout_pane_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['pane'] = $form_state['values']['pane'];
}
