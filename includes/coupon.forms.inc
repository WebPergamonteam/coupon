<?php


function coupon_form($form, &$form_state, $coupon = NULL) {
  // If $coupon is NULL, we're creating a new coupon.
  if (!$coupon) {
    $coupon = (object) array(
      'code' => coupon_generate_code(),
      'status' => COUPON_ACTIVE,
    );
  }
  
  $form['coupon_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon code'),
    '#default_value' => isset($coupon->code) ? $coupon->code : '',
    '#description' => t('The code that users can enter to redeem the coupon.'),
    '#required' => TRUE,
    '#maxlength' => 32,
  );

  $form['discount_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Discount percentage'),
    '#default_value' => isset($coupon->discount_percentage) ? $coupon->discount_percentage : '',
    '#description' => t('The percentage discount provided by the coupon.'),
    '#required' => TRUE,
    '#size' => 5,
    '#maxlength' => 5,
	'#element_validate' => array('coupon_validate_discount_percentage'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($coupon->description) ? $coupon->description : '',
    '#description' => t('A brief description of the coupon (optional).'),
    '#rows' => 4,
  );

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(
      COUPON_ACTIVE => t('Active'),
      COUPON_USED => t('Used'),
    ),
    '#default_value' => isset($coupon->status) ? $coupon->status : COUPON_ACTIVE,
    '#description' => t('The status of the coupon.'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
	
  $form['#submit'] = array('coupon_form_submit');

  return $form;
}




/**
 * Implements hook_form_submit().
 */

function coupon_form_submit($form, &$form_state) {
  // Getting the submitted values from the form state.
  $values = $form_state['values'];

  // Check if the coupon code already exists.
  $existing_coupon = coupon_load_by_code($values['coupon_code']);
if ($existing_coupon && (!isset($values['coupon_id']) || $existing_coupon->coupon_id != $values['coupon_id']))  {
    form_set_error('coupon_code', t('Coupon code already exists.'));
  }
  else {
    // Updating the coupon data in the database.
    $coupon = new stdClass();
    $coupon->coupon_code = $values['coupon_code'];
    $coupon->discount_percentage = $values['discount_percentage'];
    $coupon->description = $values['description'];
    $coupon->status = $values['status'];

    if (isset($values['coupon_id'])) {
      $coupon->coupon_id = $values['coupon_id'];
      drupal_write_record('coupon', $coupon, 'coupon_id');
    }
    else {
      drupal_write_record('coupon', $coupon);
    }

    // Redirect the user back to the coupon create page.
    drupal_set_message(t('Coupon saved successfully.'));
    drupal_goto('admin/config/coupons/create');
  }
}

/**
 * Implements hook_form_edit().
 */

function coupon_edit_form($form, &$form_state, $coupon_id) {
  $coupon = coupon_load($coupon_id);

	$form['coupon_id'] = array(
	    '#type' => 'hidden',
	    '#value' => $coupon_id,
	);

  $form['coupon_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon Code'),
    '#default_value' => $coupon->coupon_code,
    '#required' => TRUE,
    '#maxlength' => 32,
  );

  $form['discount_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Discount Percentage'),
    '#default_value' => $coupon->discount_percentage,
    '#required' => TRUE,
    '#maxlength' => 5,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $coupon->description,
    '#required' => FALSE,
  );
 
    $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(
      COUPON_ACTIVE => t('Active'),
      COUPON_USED => t('Used'),
    ),
    '#default_value' => $coupon->status,
    '#description' => t('The status of the coupon.'),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#submit'] = array('coupon_edit_form_submit');


  return $form;
}

/**
 * Implements hook_form_submit().
 */

function coupon_edit_form_submit($form, &$form_state) {
	// Getting the submitted values from the form state
	$values = $form_state['values'];
  

    // Updating the coupon data in the database
    $coupon = new stdClass();

    $coupon->coupon_code = $values['coupon_code'];
    $coupon->discount_percentage = $values['discount_percentage'];
    $coupon->description = $values['description'];
    $coupon->status = $values['status'];

	if (isset($form_state['values']['coupon_id'])) {
	  $coupon->coupon_id = $form_state['values']['coupon_id'];
	  drupal_write_record('coupon', $coupon, 'coupon_id');
	}
	else {
	  drupal_write_record('coupon', $coupon);
	}
	    // Redirecting the user back to the coupons page
	    drupal_set_message(t('Coupon edited successfully.'));
	    drupal_goto('admin/config/coupons');
}

function coupon_search_form($form, &$form_state) {
  $form = array();
  $form['#method'] = 'get';
  $form['coupon_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon Code'),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => isset($_GET['coupon_code']) ? $_GET['coupon_code'] : '',
  );
  
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  
    $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('coupon_search_form_reset'),
  );
  
  return $form;
}

function coupon_search_form_submit($form, &$form_state) {
  $coupon_code = $form_state['values']['coupon_code'];
  $query = db_select('coupon', 'c')
    ->fields('c', array('coupon_id', 'coupon_code', 'discount_percentage', 'description', 'status'))
   // ->condition('coupon_code', '%' . db_like($coupon_code) . '%', 'LIKE');
	->condition('coupon_code', $coupon_code, '=');
  $header = array(
    array('data' => t('Coupon ID'), 'field' => 'coupon_id', 'sort' => 'asc'),
    array('data' => t('Coupon Code'), 'field' => 'coupon_code'),
    array('data' => t('Discount Percentage'), 'field' => 'discount_percentage'),
    array('data' => t('Description'), 'field' => 'description'),
    array('data' => t('Status'), 'field' => 'status'),
    array('data' => t('Operations')),
  );

  $table_sort = $query->extend('TableSort')->orderByHeader($header);
  $pager = $table_sort->extend('PagerDefault')->limit(20);

  $result = $pager->execute();

  $rows = array();
  foreach ($result as $coupon) {
    $edit_link = l(t('Edit'), 'admin/config/coupons/edit/' . $coupon->coupon_id);
    $use_link = l(t('Use'), 'admin/config/coupons/use/' . $coupon->coupon_id);
    $activate_link = l(t('Activate'), 'admin/config/coupons/activate/' . $coupon->coupon_id);
    $rows[] = array(
      $coupon->coupon_id,
      $coupon->coupon_code,
      $coupon->discount_percentage . '%',
      $coupon->description,
      $coupon->status ? t('Used') : t('Active'),
	  $edit_link . ' | ' . $use_link . ' | ' . $activate_link,
    );
  }

  $output = drupal_render($form);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager');

  return $output;
}