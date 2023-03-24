<?php

/**
 * Implements hook_permission().
 */
 
function coupon_permission() {
  return array(
    'create coupons' => array(
      'title' => t('Create new coupons'),
      'description' => t('Create new discount coupons'),
    ),
    'edit coupons' => array(
      'title' => t('Edit existing coupons'),
      'description' => t('Edit existing discount coupons'),
    ),
    'view coupons' => array(
      'title' => t('View existing coupons'),
      'description' => t('View existing discount coupons'),
    ),   
    'use coupons' => array(
      'title' => t('Use discount coupons'),
      'description' => t('Use discount coupons to apply discounts'),
    ),
  );
}