CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 
 
INTRODUCTION
------------

A custom Drupal (for version 7) module Coupon that allows specific user role(s) to manage discount coupons. The module allows users to create coupons with automatically generated unique codes and specify a discount amount. Authenticated users can be able to verify the coupon code, view the discount, and mark the coupon as used.


REQUIREMENTS
------------

This module requires the following modules:

 * Entity API (https://www.drupal.org/project/entity)
 * views  (https://www.drupal.org/project/views)
 
 
INSTALLATION
------------

 * Download and install as you would normally install a contributed Drupal module.
 
 Navigate to administer >> modules. Enable Coupon module 


CONFIGURATION
-------------

 * Configure the user permissions in Administration » People » Permissions:
 * config/coupons/create - Coupon Creation page
 * admin/config/coupons - Coupons list page
 * admin/config/coupons/verify - Coupon verification and usage page


TROUBLESHOOTING
---------------

 * Additional help is available by mailing me pergamonteam@gmail.com
 