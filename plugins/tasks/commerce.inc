<?php

/**
 * @file
 * Handle the 'commerce' override task.
 *
 * This plugin overrides commerce/% and reroutes it to the page manager, where
 * a list of tasks can be used to service this request based upon criteria
 * supplied by access plugins.
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks().
 * See api-task.html for
 * more information.
 */
function commerce_panels_integration_commerce_page_manager_tasks() {
  return array(
    // This is a 'page' task and will fall under the page admin UI.
    'task type' => 'page',
    'title' => t('Commerce Checkout Template'),
    'admin title' => t('Commerce Checkout Template'),
    'admin description' => t('When enabled, this overrides the default Drupal
    behavior for displaying commerce checkout form.'),
    'admin path' => 'checkout/%commerce_order',
    // Callback to add items to the page manager task administration form:
    'hook menu' => 'commerce_panels_integration_commerce_menu',
    'hook menu alter' => 'commerce_panels_integration_commerce_menu_alter',
    /** This is a task that uses 'context' handlers and must implement these
     * to give the
     * handler data it needs.
     */
    'handler type' => 'context',
    'get arguments' => 'commerce_panels_integration_commerce_get_arguments',
    'get context placeholders' => 'commerce_panels_integration_commerce_get_contexts',
    // Allow this to be enabled or disabled.
    'disabled' => variable_get('commerce_panels_integration_commerce_disabled', TRUE),
    'enable callback' => 'commerce_panels_integration_commerce_enable',
    'access callback' => 'commerce_panels_integration_commerce_access_check',
  );
}

/**
 * Callback defined by
 * commerce_panels_integration_commerce_page_manager_tasks().
 *
 * Alter the commerce form so that view comes to us rather than the
 * normal process.
 */
function commerce_panels_integration_commerce_menu_alter(&$items, $task) {
  if (variable_get('commerce_panels_integration_commerce_disabled', TRUE)) {
    return;
  }
  if ($items['checkout/%commerce_order/%commerce_checkout_page']['page callback'] == 'drupal_get_form' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['checkout/%commerce_order/%commerce_checkout_page']['page callback'] = 'commerce_panels_integration_commerce_page';
    $items['checkout/%commerce_order/%commerce_checkout_page']['page arguments'] = array(1, 2);
    $items['checkout/%commerce_order/%commerce_checkout_page']['file path'] = $task['path'];
    $items['checkout/%commerce_order/%commerce_checkout_page']['file'] = $task['file'];
  }
  if ($items['checkout/%commerce_order']['page callback'] == 'commerce_checkout_router' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['checkout/%commerce_order']['page callback'] = 'commerce_panels_integration_commerce_page';
    $items['checkout/%commerce_order']['page arguments'] = array(1);
    $items['checkout/%commerce_order']['file path'] = $task['path'];
    $items['checkout/%commerce_order']['file'] = $task['file'];
  }
  else {
    // Automatically disable this task if it cannot be enabled.
    variable_set('commerce_panels_integration_commerce_disabled', TRUE);
    if (!empty($GLOBALS['commerce_panels_integration_enabling_commerce'])) {
      drupal_set_message(t('Page manager module is unable to enable checkout/%commerce_order because some other module already has overridden with %callback.', array('%callback' => $items['checkout/%commerce_order']['page callback'])), 'error');
    }
  }
}

/**
 * In this function we are going to call the page arguments and put it in 
 * context so we can access it all the time.
 * We build our form with the page args that are send with the page callback
 */
function commerce_panels_integration_commerce_page($order, $checkout_page = NULL) {
  // Load my task plugin:
  $task = page_manager_get_task('commerce');

  // Load the account into a context.
  ctools_include('context');
  ctools_include('context-task-handler');
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($order, $checkout_page));

  $output = ctools_context_handler_render($task, '', $contexts, array($order, $checkout_page));


  if ($output != FALSE) {
    return $output;
  }


  $function = 'drupal_get_form';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('commerce')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  if ($function == 'drupal_get_form') {

    /**
     * In order to ajax fields to work we need to run form_load_include.
     * Hence we eschew drupal_get_form and manually build the info and
     * call drupal_build_form.
     */
    $form_state = array();
    $form_id = 'commerce_form_' . $checkout_page['page_id'];
    $args = array($order, $checkout_page);
    $form_state['build_info']['args'] = $args;
    form_load_include($form_state, 'inc', 'checkout', 'commerce_pages');
    $output = drupal_build_form($form_id, $form_state);
    return $output;
  }

  return $function($order, $checkout_page = NULL);
}

/**
 * Callback to get arguments provided by this task handler.
 *
 */
function commerce_panels_integration_commerce_get_arguments($task, $subtask_id) {
  return array(
    array(
        'keyword' => 'commerce_order',
        'identifier' => t('Checkout page beging viewed'),
        'id' => 1,
        'name' => 'commerce',
        'settings' => array(),
      ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function commerce_panels_integration_commerce_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(commerce_panels_integration_commerce_get_arguments($task, $subtask_id));
}

/**
 * Callback to enable/disable the page from the UI.
 */
function commerce_panels_integration_commerce_enable($cache, $status) {
  variable_set('commerce_panels_integration_commerce_disabled', $status);
  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['commerce_panels_integration_enabling_commerce'] = TRUE;
  }
}

/**
 * @param type $task
 *
 * @param type $subtask_id
 *
 * @param type $contexts
 *
 * @return type 
 * Acess check for commerce page
 */
function commerce_panels_integration_commerce_access_check($task, $subtask_id, $contexts) {
  $context = reset($contexts);
  return commerce_access($context->data);
}

