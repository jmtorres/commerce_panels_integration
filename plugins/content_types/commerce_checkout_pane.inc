<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Commerce Checkout Pane'),
  'description' => t('A Pane from the Commerce Checkout workflow.'),
  'required context' => array(
    new ctools_context_required(t('Commerce Order'), 'commerce_order'),
//    new ctools_context_required(t('Commerce Checkout Page'), 'commerce_checkout_page'),
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
  $pane_id = $conf['pane'];
  $checkout_pane = commerce_checkout_pane_load($pane_id);

  if ($callback = commerce_checkout_pane_callback($checkout_pane, 'checkout_form')) {
    // Generate the pane form.
    $pane_form = $callback($form, $form_state, $checkout_pane, $context[0]->data);

    // Combine the messages that were created during this pane's validation or
    // submit process with any that were created during the pane generation
    // and merge them into the session's current messages array.
    if (!empty($form_state['storage']['messages'][$pane_id])) {
      $_SESSION['messages'] = array_merge_recursive($form_state['storage']['messages'][$pane_id], drupal_get_messages());
    }

    // If there are messages in the session right now for this pane, theme
    // them into the form right above the pane itself.
    if (!empty($_SESSION['messages'])) {
      // If there are error messages and this is not the first pane on the
      // form, then indicate we need to show an error message at the top of
      // the page.
      if ($visible_panes > 0 && !empty($_SESSION['messages']['error'])) {
        $show_errors_message = TRUE;
      }

      // Rendering status messages clears the session of messages, so they
      // will not be visible if the user is redirected. We can at least not
      // render here when we detect the global variable added by Rules to
      // handle redirects, though modules implementing redirects will still
      // encounter the same problem of "lost" messages.
      if (!isset($GLOBALS['_rules_action_drupal_goto_do'])) {
        $form_state['storage']['themed_messages'][$pane_id] = theme('status_messages');

        $pane_form[$pane_id . '_messages'] = array(
          '#markup' => $form_state['storage']['themed_messages'][$pane_id], 
          '#weight' => -50,
        );
      }
    }

    // Create a fieldset for the pane and add the form data defined in the
    // pane's form callback.
    if ($pane_form) {
      $form[$pane_id] = $pane_form + array(
        '#type' => $checkout_pane['fieldset'] ? 'fieldset' : 'container', 
        '#title' => check_plain($checkout_pane['title']), 
        '#collapsible' => $checkout_pane['collapsible'], 
        '#collapsed' => $checkout_pane['collapsed'], 
        '#attributes' => array('class' => array($pane_id)), 
        '#tree' => TRUE,
      );
    }
  }

  $pane = new stdClass();

  $pane->content = $pane_form;

  return $pane;
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
