-- ThunderCore Launcher Pro - Complete Database Setup

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ============================================
-- 1. Crear base de datos del launcher
-- ============================================
CREATE DATABASE IF NOT EXISTS `launcher` 
CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE `launcher`;

-- ============================================
-- 2. Tabla de configuración del sistema
-- ============================================
CREATE TABLE `system_config` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `key` VARCHAR(100) NOT NULL UNIQUE,
  `value` TEXT,
  `type` ENUM('string', 'int', 'bool', 'json', 'array') DEFAULT 'string',
  `category` VARCHAR(50) DEFAULT 'general',
  `description` TEXT,
  `is_public` BOOLEAN DEFAULT FALSE,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_key` (`key`),
  INDEX `idx_category` (`category`)
) ENGINE=InnoDB;

-- ============================================
-- 3. Tabla de usuarios del launcher
-- ============================================
CREATE TABLE `launcher_users` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `account_id` INT UNSIGNED NOT NULL UNIQUE,
  `username` VARCHAR(255) NOT NULL,
  `avatar_url` VARCHAR(500) DEFAULT NULL,
  `title` VARCHAR(100) DEFAULT NULL,
  `signature` TEXT,
  `level` INT UNSIGNED DEFAULT 1,
  `experience` BIGINT UNSIGNED DEFAULT 0,
  `reputation` INT DEFAULT 0,
  `daily_login_streak` INT UNSIGNED DEFAULT 0,
  `last_login_date` DATE DEFAULT NULL,
  `total_login_days` INT UNSIGNED DEFAULT 0,
  `total_play_time` INT UNSIGNED DEFAULT 0,
  `premium_until` DATETIME DEFAULT NULL,
  `settings` JSON,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_account` (`account_id`),
  INDEX `idx_username` (`username`),
  INDEX `idx_level` (`level`),
  INDEX `idx_premium` (`premium_until`)
) ENGINE=InnoDB;

-- ============================================
-- 4. Tabla de personajes vinculados
-- ============================================
CREATE TABLE `user_characters` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `user_id` INT UNSIGNED NOT NULL,
  `character_guid` BIGINT UNSIGNED NOT NULL,
  `character_name` VARCHAR(255) NOT NULL,
  `character_class` VARCHAR(50) NOT NULL,
  `character_race` VARCHAR(50) NOT NULL,
  `character_level` INT UNSIGNED DEFAULT 1,
  `character_guild` VARCHAR(255) DEFAULT NULL,
  `is_main` BOOLEAN DEFAULT FALSE,
  `last_seen` DATETIME DEFAULT NULL,
  `total_achievement_points` INT UNSIGNED DEFAULT 0,
  `pvp_rating` INT UNSIGNED DEFAULT 0,
  `pve_score` INT UNSIGNED DEFAULT 0,
  `gear_score` INT UNSIGNED DEFAULT 0,
  `mounts_collected` INT UNSIGNED DEFAULT 0,
  `pets_collected` INT UNSIGNED DEFAULT 0,
  `titles_collected` INT UNSIGNED DEFAULT 0,
  `snapshot` JSON,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_character` (`character_guid`),
  INDEX `idx_user` (`user_id`),
  INDEX `idx_main` (`user_id`, `is_main`),
  INDEX `idx_level` (`character_level`),
  INDEX `idx_class` (`character_class`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 5. Tabla de mensajes privados
-- ============================================
CREATE TABLE `private_messages` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `sender_id` INT UNSIGNED NOT NULL,
  `receiver_id` INT UNSIGNED NOT NULL,
  `subject` VARCHAR(255) NOT NULL,
  `message` TEXT NOT NULL,
  `is_read` BOOLEAN DEFAULT FALSE,
  `read_at` DATETIME DEFAULT NULL,
  `deleted_by_sender` BOOLEAN DEFAULT FALSE,
  `deleted_by_receiver` BOOLEAN DEFAULT FALSE,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_sender` (`sender_id`, `created_at`),
  INDEX `idx_receiver` (`receiver_id`, `created_at`, `is_read`),
  INDEX `idx_conversation` (`sender_id`, `receiver_id`, `created_at`),
  FOREIGN KEY (`sender_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`receiver_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 6. Tabla de amistades
-- ============================================
CREATE TABLE `friendships` (
  `user_id` INT UNSIGNED NOT NULL,
  `friend_id` INT UNSIGNED NOT NULL,
  `status` ENUM('pending', 'accepted', 'blocked') DEFAULT 'pending',
  `request_message` TEXT,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`user_id`, `friend_id`),
  INDEX `idx_user` (`user_id`, `status`),
  INDEX `idx_friend` (`friend_id`, `status`),
  INDEX `idx_both` (`user_id`, `friend_id`, `status`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`friend_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 7. Tabla de notificaciones
-- ============================================
CREATE TABLE `notifications` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `user_id` INT UNSIGNED NOT NULL,
  `type` VARCHAR(50) NOT NULL,
  `title` VARCHAR(255) NOT NULL,
  `content` TEXT NOT NULL,
  `is_read` BOOLEAN DEFAULT FALSE,
  `read_at` DATETIME DEFAULT NULL,
  `action_url` VARCHAR(500) DEFAULT NULL,
  `action_data` JSON,
  `expires_at` DATETIME DEFAULT NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`user_id`, `is_read`, `created_at`),
  INDEX `idx_type` (`type`, `created_at`),
  INDEX `idx_expires` (`expires_at`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 8. Tabla de logros del launcher
-- ============================================
CREATE TABLE `launcher_achievements` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `code` VARCHAR(50) UNIQUE NOT NULL,
  `name` VARCHAR(100) NOT NULL,
  `description` TEXT NOT NULL,
  `points` INT UNSIGNED NOT NULL,
  `icon_url` VARCHAR(500) DEFAULT NULL,
  `category` VARCHAR(50) NOT NULL,
  `hidden` BOOLEAN DEFAULT FALSE,
  `repeatable` BOOLEAN DEFAULT FALSE,
  `max_progress` INT UNSIGNED DEFAULT 1,
  `reward_type` VARCHAR(50) DEFAULT NULL,
  `reward_value` VARCHAR(255) DEFAULT NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_code` (`code`),
  INDEX `idx_category` (`category`)
) ENGINE=InnoDB;

-- ============================================
-- 9. Tabla de logros desbloqueados
-- ============================================
CREATE TABLE `user_achievements` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `user_id` INT UNSIGNED NOT NULL,
  `achievement_id` INT UNSIGNED NOT NULL,
  `unlocked_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `progress` INT UNSIGNED DEFAULT 0,
  `completed` BOOLEAN DEFAULT FALSE,
  `completed_at` DATETIME DEFAULT NULL,
  `times_completed` INT UNSIGNED DEFAULT 0,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_user_achievement` (`user_id`, `achievement_id`),
  INDEX `idx_user` (`user_id`, `completed`),
  INDEX `idx_achievement` (`achievement_id`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`achievement_id`) REFERENCES `launcher_achievements`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 10. Tabla de insignias
-- ============================================
CREATE TABLE `badges` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `code` VARCHAR(50) UNIQUE NOT NULL,
  `name` VARCHAR(100) NOT NULL,
  `description` TEXT,
  `icon_url` VARCHAR(500) DEFAULT NULL,
  `rarity` ENUM('common', 'uncommon', 'rare', 'epic', 'legendary') DEFAULT 'common',
  `requirements` JSON,
  `category` VARCHAR(50) DEFAULT 'general',
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_code` (`code`),
  INDEX `idx_rarity` (`rarity`)
) ENGINE=InnoDB;

-- ============================================
-- 11. Tabla de insignias de usuarios
-- ============================================
CREATE TABLE `user_badges` (
  `user_id` INT UNSIGNED NOT NULL,
  `badge_id` INT UNSIGNED NOT NULL,
  `equipped` BOOLEAN DEFAULT FALSE,
  `acquired_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `acquired_through` VARCHAR(100) DEFAULT NULL,
  PRIMARY KEY (`user_id`, `badge_id`),
  INDEX `idx_user` (`user_id`, `equipped`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`badge_id`) REFERENCES `badges`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 12. Tabla de recompensas diarias
-- ============================================
CREATE TABLE `daily_rewards` (
  `day` INT UNSIGNED NOT NULL,
  `reward_type` VARCHAR(50) NOT NULL,
  `reward_value` VARCHAR(255) NOT NULL,
  `reward_quantity` INT UNSIGNED DEFAULT 1,
  `icon_url` VARCHAR(500) DEFAULT NULL,
  `description` TEXT,
  `required_streak` INT UNSIGNED DEFAULT 0,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`day`),
  INDEX `idx_day` (`day`)
) ENGINE=InnoDB;

-- ============================================
-- 13. Tabla de progreso diario
-- ============================================
CREATE TABLE `daily_progress` (
  `user_id` INT UNSIGNED NOT NULL,
  `current_streak` INT UNSIGNED DEFAULT 0,
  `longest_streak` INT UNSIGNED DEFAULT 0,
  `last_claim_date` DATE DEFAULT NULL,
  `total_claimed` INT UNSIGNED DEFAULT 0,
  `missed_days` INT UNSIGNED DEFAULT 0,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`user_id`),
  INDEX `idx_streak` (`current_streak`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 14. Tabla de misiones del launcher
-- ============================================
CREATE TABLE `launcher_quests` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `code` VARCHAR(50) UNIQUE NOT NULL,
  `title` VARCHAR(255) NOT NULL,
  `description` TEXT NOT NULL,
  `type` ENUM('daily', 'weekly', 'monthly', 'special') DEFAULT 'daily',
  `objectives` JSON NOT NULL,
  `rewards` JSON NOT NULL,
  `required_level` INT UNSIGNED DEFAULT 1,
  `max_completions` INT UNSIGNED DEFAULT 1,
  `cooldown_hours` INT UNSIGNED DEFAULT 24,
  `start_date` DATE DEFAULT NULL,
  `end_date` DATE DEFAULT NULL,
  `active` BOOLEAN DEFAULT TRUE,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_code` (`code`),
  INDEX `idx_type` (`type`, `active`),
  INDEX `idx_dates` (`start_date`, `end_date`)
) ENGINE=InnoDB;

-- ============================================
-- 15. Tabla de progreso de misiones
-- ============================================
CREATE TABLE `user_quests` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `user_id` INT UNSIGNED NOT NULL,
  `quest_id` INT UNSIGNED NOT NULL,
  `progress` JSON,
  `completed` BOOLEAN DEFAULT FALSE,
  `completed_at` DATETIME DEFAULT NULL,
  `claimed` BOOLEAN DEFAULT FALSE,
  `claimed_at` DATETIME DEFAULT NULL,
  `abandoned` BOOLEAN DEFAULT FALSE,
  `started_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_user_quest` (`user_id`, `quest_id`),
  INDEX `idx_user` (`user_id`, `completed`, `claimed`),
  INDEX `idx_quest` (`quest_id`, `completed`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`quest_id`) REFERENCES `launcher_quests`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 16. Tabla de tickets de soporte
-- ============================================
CREATE TABLE `support_tickets` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `ticket_number` VARCHAR(20) UNIQUE NOT NULL,
  `user_id` INT UNSIGNED NOT NULL,
  `category` VARCHAR(50) NOT NULL,
  `subject` VARCHAR(255) NOT NULL,
  `message` TEXT NOT NULL,
  `status` ENUM('open', 'in_progress', 'resolved', 'closed') DEFAULT 'open',
  `priority` ENUM('low', 'medium', 'high', 'critical') DEFAULT 'medium',
  `assigned_to` INT UNSIGNED DEFAULT NULL,
  `assigned_at` DATETIME DEFAULT NULL,
  `resolved_at` DATETIME DEFAULT NULL,
  `closed_at` DATETIME DEFAULT NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_user_status` (`user_id`, `status`),
  INDEX `idx_status` (`status`),
  INDEX `idx_priority` (`priority`),
  INDEX `idx_assigned` (`assigned_to`, `status`),
  INDEX `idx_created` (`created_at`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 17. Tabla de respuestas a tickets
-- ============================================
CREATE TABLE `ticket_replies` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `ticket_id` BIGINT UNSIGNED NOT NULL,
  `user_id` INT UNSIGNED NOT NULL,
  `message` TEXT NOT NULL,
  `is_staff` BOOLEAN DEFAULT FALSE,
  `is_internal` BOOLEAN DEFAULT FALSE,
  `attachments` JSON,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_ticket` (`ticket_id`, `created_at`),
  INDEX `idx_user` (`user_id`),
  FOREIGN KEY (`ticket_id`) REFERENCES `support_tickets`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 18. Tabla de eventos del servidor
-- ============================================
CREATE TABLE `server_events` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `event_code` VARCHAR(50) UNIQUE NOT NULL,
  `title` VARCHAR(255) NOT NULL,
  `description` TEXT,
  `event_type` VARCHAR(50) NOT NULL,
  `start_time` DATETIME NOT NULL,
  `end_time` DATETIME NOT NULL,
  `registration_start` DATETIME DEFAULT NULL,
  `registration_end` DATETIME DEFAULT NULL,
  `created_by` INT UNSIGNED DEFAULT NULL,
  `max_participants` INT UNSIGNED DEFAULT 0,
  `current_participants` INT UNSIGNED DEFAULT 0,
  `status` ENUM('scheduled', 'registration_open', 'ongoing', 'completed', 'cancelled') DEFAULT 'scheduled',
  `requirements` JSON,
  `rewards` JSON,
  `rules` TEXT,
  `location` VARCHAR(255),
  `stream_url` VARCHAR(500) DEFAULT NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_time` (`start_time`, `end_time`),
  INDEX `idx_status` (`status`),
  INDEX `idx_type` (`event_type`)
) ENGINE=InnoDB;

-- ============================================
-- 19. Tabla de participantes de eventos
-- ============================================
CREATE TABLE `event_participants` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `event_id` INT UNSIGNED NOT NULL,
  `user_id` INT UNSIGNED NOT NULL,
  `character_guid` BIGINT UNSIGNED DEFAULT NULL,
  `character_name` VARCHAR(255) DEFAULT NULL,
  `character_class` VARCHAR(50) DEFAULT NULL,
  `character_level` INT UNSIGNED DEFAULT NULL,
  `status` ENUM('registered', 'checked_in', 'participated', 'disqualified', 'won') DEFAULT 'registered',
  `team` VARCHAR(100) DEFAULT NULL,
  `score` INT DEFAULT 0,
  `placement` INT UNSIGNED DEFAULT NULL,
  `reward_received` BOOLEAN DEFAULT FALSE,
  `registered_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `checked_in_at` DATETIME DEFAULT NULL,
  `participated_at` DATETIME DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_event_user` (`event_id`, `user_id`),
  INDEX `idx_event` (`event_id`, `status`),
  INDEX `idx_user` (`user_id`),
  INDEX `idx_character` (`character_guid`),
  FOREIGN KEY (`event_id`) REFERENCES `server_events`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 20. Tabla de items de la tienda
-- ============================================
CREATE TABLE `store_items` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `item_code` VARCHAR(50) UNIQUE NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `description` TEXT,
  `icon_url` VARCHAR(500) DEFAULT NULL,
  `category` VARCHAR(50) NOT NULL,
  `type` VARCHAR(50) NOT NULL,
  `data` JSON NOT NULL,
  `price` DECIMAL(10, 2) NOT NULL,
  `currency` VARCHAR(10) DEFAULT 'USD',
  `stock` INT UNSIGNED DEFAULT NULL,
  `max_per_user` INT UNSIGNED DEFAULT 1,
  `limited_time` BOOLEAN DEFAULT FALSE,
  `available_from` DATETIME DEFAULT NULL,
  `available_to` DATETIME DEFAULT NULL,
  `featured` BOOLEAN DEFAULT FALSE,
  `sort_order` INT UNSIGNED DEFAULT 0,
  `active` BOOLEAN DEFAULT TRUE,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_category` (`category`, `active`),
  INDEX `idx_type` (`type`),
  INDEX `idx_featured` (`featured`),
  INDEX `idx_available` (`available_from`, `available_to`, `active`)
) ENGINE=InnoDB;

-- ============================================
-- 21. Tabla de órdenes
-- ============================================
CREATE TABLE `store_orders` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `order_number` VARCHAR(20) UNIQUE NOT NULL,
  `user_id` INT UNSIGNED NOT NULL,
  `total_amount` DECIMAL(10, 2) NOT NULL,
  `currency` VARCHAR(10) DEFAULT 'USD',
  `discount_code` VARCHAR(50) DEFAULT NULL,
  `discount_amount` DECIMAL(10, 2) DEFAULT 0,
  `final_amount` DECIMAL(10, 2) NOT NULL,
  `status` ENUM('pending', 'paid', 'processing', 'completed', 'cancelled', 'refunded') DEFAULT 'pending',
  `payment_method` VARCHAR(50) DEFAULT NULL,
  `payment_id` VARCHAR(255) DEFAULT NULL,
  `items` JSON NOT NULL,
  `character_name` VARCHAR(255) DEFAULT NULL,
  `notes` TEXT,
  `delivered` BOOLEAN DEFAULT FALSE,
  `delivered_at` DATETIME DEFAULT NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`user_id`, `created_at`),
  INDEX `idx_status` (`status`),
  INDEX `idx_payment` (`payment_method`, `payment_id`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 22. Tabla de cupones de descuento
-- ============================================
CREATE TABLE `store_coupons` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `code` VARCHAR(50) UNIQUE NOT NULL,
  `description` TEXT,
  `discount_type` ENUM('percentage', 'fixed', 'free_shipping') DEFAULT 'percentage',
  `discount_value` DECIMAL(10, 2) NOT NULL,
  `minimum_order` DECIMAL(10, 2) DEFAULT 0,
  `maximum_discount` DECIMAL(10, 2) DEFAULT NULL,
  `usage_limit` INT UNSIGNED DEFAULT NULL,
  `usage_count` INT UNSIGNED DEFAULT 0,
  `user_limit` INT UNSIGNED DEFAULT 1,
  `valid_from` DATETIME DEFAULT NULL,
  `valid_to` DATETIME DEFAULT NULL,
  `active` BOOLEAN DEFAULT TRUE,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_code` (`code`, `active`),
  INDEX `idx_validity` (`valid_from`, `valid_to`)
) ENGINE=InnoDB;

-- ============================================
-- 23. Tabla de historial de cupones usados
-- ============================================
CREATE TABLE `store_coupon_usage` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `coupon_id` INT UNSIGNED NOT NULL,
  `user_id` INT UNSIGNED NOT NULL,
  `order_id` BIGINT UNSIGNED NOT NULL,
  `discount_amount` DECIMAL(10, 2) NOT NULL,
  `used_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_coupon_order` (`coupon_id`, `order_id`),
  INDEX `idx_coupon` (`coupon_id`),
  INDEX `idx_user` (`user_id`),
  FOREIGN KEY (`coupon_id`) REFERENCES `store_coupons`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`order_id`) REFERENCES `store_orders`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 24. Tabla de wishlist
-- ============================================
CREATE TABLE `store_wishlist` (
  `user_id` INT UNSIGNED NOT NULL,
  `item_id` INT UNSIGNED NOT NULL,
  `added_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `notes` TEXT,
  `priority` INT UNSIGNED DEFAULT 1,
  PRIMARY KEY (`user_id`, `item_id`),
  INDEX `idx_user` (`user_id`, `added_at`),
  INDEX `idx_item` (`item_id`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`item_id`) REFERENCES `store_items`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 25. Tabla de referidos
-- ============================================
CREATE TABLE `referrals` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `referrer_id` INT UNSIGNED NOT NULL,
  `referred_id` INT UNSIGNED NOT NULL,
  `referred_email` VARCHAR(255) NOT NULL,
  `status` ENUM('pending', 'registered', 'active', 'rewarded') DEFAULT 'pending',
  `reward_given` BOOLEAN DEFAULT FALSE,
  `reward_amount` DECIMAL(10, 2) DEFAULT 0,
  `referred_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `registered_at` DATETIME DEFAULT NULL,
  `became_active_at` DATETIME DEFAULT NULL,
  `rewarded_at` DATETIME DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_referred` (`referred_email`),
  INDEX `idx_referrer` (`referrer_id`, `status`),
  INDEX `idx_referred` (`referred_id`),
  FOREIGN KEY (`referrer_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`referred_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 26. Tabla de estadísticas del servidor
-- ============================================
CREATE TABLE `server_statistics` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `timestamp` DATETIME NOT NULL,
  `online_players` INT UNSIGNED NOT NULL,
  `alliance_players` INT UNSIGNED DEFAULT 0,
  `horde_players` INT UNSIGNED DEFAULT 0,
  `new_accounts` INT UNSIGNED DEFAULT 0,
  `active_accounts` INT UNSIGNED DEFAULT 0,
  `characters_created` INT UNSIGNED DEFAULT 0,
  `characters_deleted` INT UNSIGNED DEFAULT 0,
  `gold_in_economy` BIGINT UNSIGNED DEFAULT 0,
  `auctions_posted` INT UNSIGNED DEFAULT 0,
  `mail_sent` INT UNSIGNED DEFAULT 0,
  `instance_runs` INT UNSIGNED DEFAULT 0,
  `battlegrounds_played` INT UNSIGNED DEFAULT 0,
  `arena_matches` INT UNSIGNED DEFAULT 0,
  `average_latency` INT UNSIGNED DEFAULT 0,
  `server_uptime` INT UNSIGNED DEFAULT 0,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_timestamp` (`timestamp`),
  INDEX `idx_date` (DATE(`timestamp`))
) ENGINE=InnoDB;

-- ============================================
-- 27. Tabla de logs de auditoría
-- ============================================
CREATE TABLE `audit_logs` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `user_id` INT UNSIGNED DEFAULT NULL,
  `action` VARCHAR(100) NOT NULL,
  `details` JSON,
  `ip_address` VARCHAR(45) DEFAULT NULL,
  `user_agent` TEXT,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`user_id`, `created_at`),
  INDEX `idx_action` (`action`, `created_at`),
  INDEX `idx_ip` (`ip_address`, `created_at`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE SET NULL
) ENGINE=InnoDB;

-- ============================================
-- 28. Tabla de sesiones
-- ============================================
CREATE TABLE `user_sessions` (
  `id` VARCHAR(64) NOT NULL,
  `user_id` INT UNSIGNED NOT NULL,
  `ip_address` VARCHAR(45) DEFAULT NULL,
  `user_agent` TEXT,
  `payload` TEXT,
  `last_activity` INT UNSIGNED NOT NULL,
  `expires_at` DATETIME NOT NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`user_id`),
  INDEX `idx_expires` (`expires_at`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 29. Tabla de bloqueos
-- ============================================
CREATE TABLE `user_blocks` (
  `user_id` INT UNSIGNED NOT NULL,
  `blocked_user_id` INT UNSIGNED NOT NULL,
  `reason` TEXT,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`user_id`, `blocked_user_id`),
  INDEX `idx_user` (`user_id`),
  INDEX `idx_blocked` (`blocked_user_id`),
  FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE,
  FOREIGN KEY (`blocked_user_id`) REFERENCES `launcher_users`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- ============================================
-- 30. Tabla de noticias
-- ============================================
CREATE TABLE `news_articles` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `title` VARCHAR(255) NOT NULL,
  `content` TEXT NOT NULL,
  `author_id` INT UNSIGNED DEFAULT NULL,
  `category` VARCHAR(50) DEFAULT 'general',
  `tags` JSON,
  `image_url` VARCHAR(500) DEFAULT NULL,
  `featured` BOOLEAN DEFAULT FALSE,
  `pinned` BOOLEAN DEFAULT FALSE,
  `views` INT UNSIGNED DEFAULT 0,
  `published` BOOLEAN DEFAULT TRUE,
  `published_at` DATETIME DEFAULT NULL,
  `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `idx_published` (`published`, `published_at`),
  INDEX `idx_featured` (`featured`),
  INDEX `idx_category` (`category`),
  FOREIGN KEY (`author_id`) REFERENCES `launcher_users`(`id`) ON DELETE SET NULL
) ENGINE=InnoDB;

-- ============================================
-- INSERTAR DATOS INICIALES
-- ============================================

-- Configuración del sistema
INSERT INTO `system_config` (`key`, `value`, `type`, `category`, `description`) VALUES
('launcher.version', '1.0.0', 'string', 'system', 'Versión actual del launcher'),
('launcher.name', 'ThunderCore Launcher Pro', 'string', 'system', 'Nombre del launcher'),
('launcher.contact_email', 'support@thundercore.com', 'string', 'system', 'Email de contacto'),
('launcher.website', 'https://thundercore.com', 'string', 'system', 'Sitio web oficial'),
('launcher.discord', 'https://discord.gg/thundernet', 'string', 'system', 'Enlace a Discord'),
('launcher.default_expansion', 'WOTLK', 'string', 'game', 'Expansión por defecto'),
('launcher.default_emulator', 'TrinityCore', 'string', 'game', 'Emulador por defecto'),
('store.currency', 'USD', 'string', 'store', 'Moneda por defecto'),
('store.tax_rate', '0.0', 'double', 'store', 'Tasa de impuestos'),
('social.enable_friends', 'true', 'bool', 'social', 'Habilitar sistema de amigos'),
('social.enable_messages', 'true', 'bool', 'social', 'Habilitar mensajería'),
('notifications.enabled', 'true', 'bool', 'notifications', 'Habilitar notificaciones'),
('updates.auto_check', 'true', 'bool', 'updates', 'Buscar actualizaciones automáticamente');

-- Logros por defecto
INSERT INTO `launcher_achievements` (`code`, `name`, `description`, `points`, `category`, `reward_type`, `reward_value`) VALUES
-- Logros de inicio de sesión
('LOGIN_FIRST', 'Primera Aventura', 'Inicia sesión por primera vez', 10, 'general', 'points', '100'),
('LOGIN_7_DAYS', 'Veterano Semanal', 'Inicia sesión 7 días seguidos', 50, 'general', 'badge', 'veteran_week'),
('LOGIN_30_DAYS', 'Leyenda Mensual', 'Inicia sesión 30 días seguidos', 200, 'general', 'title', 'El Persistente'),

-- Logros sociales
('FRIEND_FIRST', 'Amistad Forjada', 'Añade a tu primer amigo', 20, 'social', 'points', '50'),
('FRIEND_10', 'Popular', 'Consigue 10 amigos', 100, 'social', 'badge', 'popular'),
('MESSAGE_FIRST', 'Comunicador', 'Envía tu primer mensaje', 15, 'social', 'points', '25'),

-- Logros de perfil
('PROFILE_COMPLETE', 'Perfil Maestro', 'Completa tu perfil al 100%', 30, 'profile', 'title', 'El Detallista'),

-- Logros de eventos
('EVENT_PARTICIPATE', 'Participante', 'Participa en un evento del servidor', 40, 'events', 'points', '75'),
('EVENT_WIN_FIRST', 'Campeón Novato', 'Gana tu primer evento', 100, 'events', 'badge', 'champion'),

-- Logros de launcher
('LAUNCHER_LEVEL_10', 'Aprendiz', 'Alcanza el nivel 10 en el launcher', 150, 'level', 'title', 'El Aprendiz'),
('LAUNCHER_LEVEL_25', 'Experto', 'Alcanza el nivel 25 en el launcher', 300, 'level', 'badge', 'expert'),
('LAUNCHER_LEVEL_50', 'Maestro', 'Alcanza el nivel 50 en el launcher', 500, 'level', 'title', 'El Maestro'),

-- Logros de tienda
('STORE_FIRST_PURCHASE', 'Cliente', 'Realiza tu primera compra', 25, 'store', 'points', '50'),
('STORE_SPEND_100', 'Inversionista', 'Gasta 100$ en la tienda', 150, 'store', 'badge', 'investor'),

-- Logros de juego
('CHARACTER_MAX_LEVEL', 'Héroe Máximo', 'Lleva un personaje al nivel máximo', 200, 'game', 'title', 'El Máximo'),
('ACHIEVEMENT_1000', 'Logrero', 'Consigue 1000 puntos de logros en WoW', 100, 'game', 'badge', 'achiever'),

-- Logros especiales
('REFERRAL_FIRST', 'Embajador', 'Refiere a un amigo que se registre', 75, 'special', 'points', '100'),
('BUG_REPORT', 'Cazador de Bugs', 'Reporta un bug útil', 50, 'special', 'badge', 'bug_hunter');

-- Insignias por defecto
INSERT INTO `badges` (`code`, `name`, `description`, `rarity`, `requirements`) VALUES
-- Common
('welcome', 'Bienvenido', 'Nuevo en ThunderCore', 'common', '{"type": "auto", "condition": "new_user"}'),
('login_week', 'Semana Activa', '7 días consecutivos de login', 'common', '{"type": "login_streak", "days": 7}'),
('first_message', 'Primer Mensaje', 'Envía tu primer mensaje', 'common', '{"type": "first_message"}'),

-- Uncommon
('friend_maker', 'Hacedor de Amigos', '10 amigos añadidos', 'uncommon', '{"type": "friend_count", "count": 10}'),
('event_participant', 'Participante de Eventos', 'Participa en 5 eventos', 'uncommon', '{"type": "event_participations", "count": 5}'),
('profile_complete', 'Perfil Completo', 'Perfil completado al 100%', 'uncommon', '{"type": "profile_completion"}'),

-- Rare
('veteran_month', 'Veterano Mensual', '30 días consecutivos de login', 'rare', '{"type": "login_streak", "days": 30}'),
('store_customer', 'Cliente Frecuente', '5 compras en la tienda', 'rare', '{"type": "store_purchases", "count": 5}'),
('achievement_hunter', 'Cazador de Logros', '10 logros desbloqueados', 'rare', '{"type": "achievement_count", "count": 10}'),

-- Epic
('legendary_player', 'Jugador Legendario', 'Nivel 50 en el launcher', 'epic', '{"type": "launcher_level", "level": 50}'),
('event_champion', 'Campeón de Eventos', 'Gana 3 eventos', 'epic', '{"type": "event_wins", "count": 3}'),
('top_referrer', 'Top Referidor', 'Refiere a 10 amigos', 'epic', '{"type": "referral_count", "count": 10}'),

-- Legendary
('founder', 'Fundador', 'Usuario desde el lanzamiento', 'legendary', '{"type": "account_age", "days": 365}'),
('community_leader', 'Líder de la Comunidad', 'Reconocido por la comunidad', 'legendary', '{"type": "staff_granted"}'),
('ultimate_supporter', 'Soporte Definitivo', 'Gran contribuidor al servidor', 'legendary', '{"type": "special_achievement"}');

-- Recompensas diarias (ciclo de 30 días)
INSERT INTO `daily_rewards` (`day`, `reward_type`, `reward_value`, `reward_quantity`, `description`) VALUES
(1, 'points', '50', 1, '50 Puntos de Experiencia'),
(2, 'points', '75', 1, '75 Puntos de Experiencia'),
(3, 'points', '100', 1, '100 Puntos de Experiencia'),
(4, 'badge', 'login_week', 1, 'Insignia: Semana Activa'),
(5, 'points', '150', 1, '150 Puntos de Experiencia'),
(6, 'title', 'El Persistente', 1, 'Título: El Persistente'),
(7, 'points', '200', 1, '200 Puntos de Experiencia'),
(8, 'points', '100', 1, '100 Puntos de Experiencia'),
(9, 'points', '125', 1, '125 Puntos de Experiencia'),
(10, 'badge', 'veteran_month', 1, 'Insignia: Veterano Mensual'),
(11, 'points', '150', 1, '150 Puntos de Experiencia'),
(12, 'title', 'El Constante', 1, 'Título: El Constante'),
(13, 'points', '175', 1, '175 Puntos de Experiencia'),
(14, 'points', '200', 1, '200 Puntos de Experiencia'),
(15, 'badge', 'legendary_player', 1, 'Insignia: Jugador Legendario'),
(16, 'points', '250', 1, '250 Puntos de Experiencia'),
(17, 'title', 'El Incansable', 1, 'Título: El Incansable'),
(18, 'points', '300', 1, '300 Puntos de Experiencia'),
(19, 'points', '350', 1, '350 Puntos de Experiencia'),
(20, 'badge', 'ultimate_supporter', 1, 'Insignia: Soporte Definitivo'),
(21, 'points', '400', 1, '400 Puntos de Experiencia'),
(22, 'title', 'El Épico', 1, 'Título: El Épico'),
(23, 'points', '450', 1, '450 Puntos de Experiencia'),
(24, 'points', '500', 1, '500 Puntos de Experiencia'),
(25, 'badge', 'founder', 1, 'Insignia: Fundador'),
(26, 'points', '600', 1, '600 Puntos de Experiencia'),
(27, 'title', 'El Legendario', 1, 'Título: El Legendario'),
(28, 'points', '700', 1, '700 Puntos de Experiencia'),
(29, 'points', '800', 1, '800 Puntos de Experiencia'),
(30, 'badge', 'community_leader', 1, 'Insignia: Líder de la Comunidad');

-- Cupones de bienvenida
INSERT INTO `store_coupons` (`code`, `description`, `discount_type`, `discount_value`, `usage_limit`, `valid_from`, `valid_to`) VALUES
('WELCOME10', '10% de descuento en tu primera compra', 'percentage', 10.00, 1, NOW(), DATE_ADD(NOW(), INTERVAL 30 DAY)),
('NEWPLAYER5', '5$ de descuento para nuevos jugadores', 'fixed', 5.00, 1, NOW(), DATE_ADD(NOW(), INTERVAL 60 DAY)),
('LAUNCHER25', '25% de descuento especial de lanzamiento', 'percentage', 25.00, 100, NOW(), DATE_ADD(NOW(), INTERVAL 90 DAY));

SET FOREIGN_KEY_CHECKS = 1;

-- ============================================
-- CREAR USUARIOS Y PERMISOS
-- ============================================

-- Usuario para la aplicación (modificar contraseña en producción)
CREATE USER IF NOT EXISTS 'launcher_app'@'localhost' IDENTIFIED BY 'ThunderCoreSecure2024!';
GRANT SELECT, INSERT, UPDATE, DELETE, EXECUTE ON `launcher`.* TO 'launcher_app'@'localhost';

-- Usuario para backups (solo lectura)
CREATE USER IF NOT EXISTS 'launcher_backup'@'localhost' IDENTIFIED BY 'BackupPass2024!';
GRANT SELECT, SHOW VIEW, LOCK TABLES, RELOAD ON `launcher`.* TO 'launcher_backup'@'localhost';

FLUSH PRIVILEGES;

-- ============================================
-- PROCEDIMIENTOS ALMACENADOS
-- ============================================

DELIMITER $$

-- Procedimiento para registrar login diario
CREATE PROCEDURE `RegisterDailyLogin`(
    IN p_account_id INT UNSIGNED,
    IN p_user_id INT UNSIGNED
)
BEGIN
    DECLARE v_last_login DATE;
    DECLARE v_current_streak INT;
    DECLARE v_longest_streak INT;
    DECLARE v_yesterday DATE;
    
    SET v_yesterday = DATE_SUB(CURDATE(), INTERVAL 1 DAY);
    
    -- Obtener último login y racha actual
    SELECT last_claim_date, current_streak, longest_streak
    INTO v_last_login, v_current_streak, v_longest_streak
    FROM daily_progress
    WHERE user_id = p_user_id;
    
    IF v_last_login IS NULL THEN
        -- Primer login
        INSERT INTO daily_progress (user_id, current_streak, longest_streak, last_claim_date, total_claimed)
        VALUES (p_user_id, 1, 1, CURDATE(), 1);
        
        -- Otorgar logro de primer login
        INSERT INTO user_achievements (user_id, achievement_id, completed, completed_at)
        SELECT p_user_id, id, TRUE, NOW()
        FROM launcher_achievements
        WHERE code = 'LOGIN_FIRST'
        ON DUPLICATE KEY UPDATE times_completed = times_completed + 1;
        
    ELSEIF v_last_login = CURDATE() THEN
        -- Ya reclamado hoy
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Daily reward already claimed today';
        
    ELSEIF v_last_login = v_yesterday THEN
        -- Login consecutivo
        SET v_current_streak = v_current_streak + 1;
        IF v_current_streak > v_longest_streak THEN
            SET v_longest_streak = v_current_streak;
        END IF;
        
        UPDATE daily_progress 
        SET current_streak = v_current_streak,
            longest_streak = v_longest_streak,
            last_claim_date = CURDATE(),
            total_claimed = total_claimed + 1,
            missed_days = 0
        WHERE user_id = p_user_id;
        
        -- Otorgar logros de racha
        IF v_current_streak = 7 THEN
            INSERT INTO user_achievements (user_id, achievement_id, completed, completed_at)
            SELECT p_user_id, id, TRUE, NOW()
            FROM launcher_achievements
            WHERE code = 'LOGIN_7_DAYS'
            ON DUPLICATE KEY UPDATE times_completed = times_completed + 1;
        END IF;
        
        IF v_current_streak = 30 THEN
            INSERT INTO user_achievements (user_id, achievement_id, completed, completed_at)
            SELECT p_user_id, id, TRUE, NOW()
            FROM launcher_achievements
            WHERE code = 'LOGIN_30_DAYS'
            ON DUPLICATE KEY UPDATE times_completed = times_completed + 1;
        END IF;
        
    ELSE
        -- Rompió la racha
        UPDATE daily_progress 
        SET current_streak = 1,
            last_claim_date = CURDATE(),
            total_claimed = total_claimed + 1,
            missed_days = DATEDIFF(CURDATE(), v_last_login) - 1
        WHERE user_id = p_user_id;
    END IF;
    
    -- Actualizar usuario
    UPDATE launcher_users 
    SET last_login_date = CURDATE(),
        total_login_days = total_login_days + 1
    WHERE id = p_user_id;
    
    -- Registrar en auditoría
    INSERT INTO audit_logs (user_id, action, details)
    VALUES (p_user_id, 'daily_login', 
            JSON_OBJECT('account_id', p_account_id, 'streak', v_current_streak));
END$$

-- Procedimiento para enviar mensaje privado
CREATE PROCEDURE `SendPrivateMessage`(
    IN p_sender_id INT UNSIGNED,
    IN p_receiver_id INT UNSIGNED,
    IN p_subject VARCHAR(255),
    IN p_message TEXT
)
BEGIN
    DECLARE v_message_id BIGINT UNSIGNED;
    DECLARE v_receiver_exists BOOLEAN;
    
    -- Verificar que el receptor existe
    SELECT COUNT(*) > 0 INTO v_receiver_exists
    FROM launcher_users
    WHERE id = p_receiver_id;
    
    IF NOT v_receiver_exists THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Receiver does not exist';
    END IF;
    
    -- Verificar si el receptor ha bloqueado al remitente
    IF EXISTS (
        SELECT 1 FROM user_blocks 
        WHERE user_id = p_receiver_id AND blocked_user_id = p_sender_id
    ) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'You are blocked by this user';
    END IF;
    
    -- Insertar mensaje
    INSERT INTO private_messages (sender_id, receiver_id, subject, message)
    VALUES (p_sender_id, p_receiver_id, p_subject, p_message);
    
    SET v_message_id = LAST_INSERT_ID();
    
    -- Crear notificación para el receptor
    INSERT INTO notifications (user_id, type, title, content, action_url)
    VALUES (p_receiver_id, 'message', 
            CONCAT('Nuevo mensaje de ', (SELECT username FROM launcher_users WHERE id = p_sender_id)),
            p_subject,
            CONCAT('/messages/', v_message_id));
    
    -- Otorgar logro de primer mensaje si es el primero
    IF NOT EXISTS (
        SELECT 1 FROM private_messages 
        WHERE sender_id = p_sender_id AND id < v_message_id
    ) THEN
        INSERT INTO user_achievements (user_id, achievement_id, completed, completed_at)
        SELECT p_sender_id, id, TRUE, NOW()
        FROM launcher_achievements
        WHERE code = 'MESSAGE_FIRST'
        ON DUPLICATE KEY UPDATE times_completed = times_completed + 1;
    END IF;
    
    -- Registrar en auditoría
    INSERT INTO audit_logs (user_id, action, details)
    VALUES (p_sender_id, 'send_message', 
            JSON_OBJECT('receiver_id', p_receiver_id, 'message_id', v_message_id));
END$$

-- Procedimiento para procesar una compra
CREATE PROCEDURE `ProcessStoreOrder`(
    IN p_user_id INT UNSIGNED,
    IN p_items JSON,
    IN p_discount_code VARCHAR(50),
    IN p_character_name VARCHAR(255)
)
BEGIN
    DECLARE v_order_id BIGINT UNSIGNED;
    DECLARE v_total_amount DECIMAL(10,2) DEFAULT 0;
    DECLARE v_discount_amount DECIMAL(10,2) DEFAULT 0;
    DECLARE v_final_amount DECIMAL(10,2);
    DECLARE v_item_count INT;
    DECLARE i INT DEFAULT 0;
    DECLARE v_item_id INT;
    DECLARE v_quantity INT;
    DECLARE v_price DECIMAL(10,2);
    DECLARE v_item_stock INT;
    DECLARE v_coupon_valid BOOLEAN;
    DECLARE v_order_number VARCHAR(20);
    
    -- Generar número de orden único
    SET v_order_number = CONCAT('ORD-', DATE_FORMAT(NOW(), '%Y%m%d-'), 
                                LPAD(FLOOR(RAND() * 10000), 4, '0'));
    
    -- Calcular total y verificar stock
    SET v_item_count = JSON_LENGTH(p_items);
    
    WHILE i < v_item_count DO
        SET v_item_id = JSON_EXTRACT(p_items, CONCAT('$[', i, '].item_id'));
        SET v_quantity = JSON_EXTRACT(p_items, CONCAT('$[', i, '].quantity'));
        
        -- Verificar stock
        SELECT stock, price INTO v_item_stock, v_price
        FROM store_items
        WHERE id = v_item_id AND active = TRUE;
        
        IF v_item_stock IS NULL THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = CONCAT('Item ', v_item_id, ' not available');
        END IF;
        
        IF v_item_stock IS NOT NULL AND v_item_stock < v_quantity THEN
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = CONCAT('Insufficient stock for item ', v_item_id);
        END IF;
        
        SET v_total_amount = v_total_amount + (v_price * v_quantity);
        SET i = i + 1;
    END WHILE;
    
    -- Aplicar descuento si existe
    IF p_discount_code IS NOT NULL THEN
        SELECT 
            CASE 
                WHEN valid_from <= NOW() AND (valid_to IS NULL OR valid_to >= NOW())
                     AND (usage_limit IS NULL OR usage_count < usage_limit)
                THEN TRUE
                ELSE FALSE
            END INTO v_coupon_valid
        FROM store_coupons
        WHERE code = p_discount_code AND active = TRUE;
        
        IF v_coupon_valid THEN
            SELECT 
                CASE discount_type
                    WHEN 'percentage' THEN v_total_amount * (discount_value / 100)
                    WHEN 'fixed' THEN discount_value
                    ELSE 0
                END INTO v_discount_amount
            FROM store_coupons
            WHERE code = p_discount_code;
            
            -- Verificar mínimo de orden
            IF EXISTS (
                SELECT 1 FROM store_coupons 
                WHERE code = p_discount_code AND minimum_order > v_total_amount
            ) THEN
                SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Minimum order amount not met';
            END IF;
            
            -- Aplicar máximo de descuento
            IF EXISTS (
                SELECT 1 FROM store_coupons 
                WHERE code = p_discount_code AND maximum_discount IS NOT NULL 
                AND v_discount_amount > maximum_discount
            ) THEN
                SELECT maximum_discount INTO v_discount_amount
                FROM store_coupons
                WHERE code = p_discount_code;
            END IF;
        END IF;
    END IF;
    
    SET v_final_amount = v_total_amount - v_discount_amount;
    IF v_final_amount < 0 THEN SET v_final_amount = 0; END IF;
    
    -- Crear orden
    INSERT INTO store_orders (
        order_number, user_id, total_amount, discount_code, 
        discount_amount, final_amount, items, character_name
    ) VALUES (
        v_order_number, p_user_id, v_total_amount, p_discount_code,
        v_discount_amount, v_final_amount, p_items, p_character_name
    );
    
    SET v_order_id = LAST_INSERT_ID();
    
    -- Actualizar stock
    SET i = 0;
    WHILE i < v_item_count DO
        SET v_item_id = JSON_EXTRACT(p_items, CONCAT('$[', i, '].item_id'));
        SET v_quantity = JSON_EXTRACT(p_items, CONCAT('$[', i, '].quantity'));
        
        UPDATE store_items
        SET stock = stock - v_quantity
        WHERE id = v_item_id AND stock IS NOT NULL;
        
        SET i = i + 1;
    END WHILE;
    
    -- Registrar uso de cupón
    IF p_discount_code IS NOT NULL AND v_coupon_valid THEN
        INSERT INTO store_coupon_usage (coupon_id, user_id, order_id, discount_amount)
        SELECT id, p_user_id, v_order_id, v_discount_amount
        FROM store_coupons
        WHERE code = p_discount_code;
        
        UPDATE store_coupons
        SET usage_count = usage_count + 1
        WHERE code = p_discount_code;
    END IF;
    
    -- Otorgar logro de primera compra si es la primera
    IF NOT EXISTS (
        SELECT 1 FROM store_orders 
        WHERE user_id = p_user_id AND id < v_order_id
    ) THEN
        INSERT INTO user_achievements (user_id, achievement_id, completed, completed_at)
        SELECT p_user_id, id, TRUE, NOW()
        FROM launcher_achievements
        WHERE code = 'STORE_FIRST_PURCHASE'
        ON DUPLICATE KEY UPDATE times_completed = times_completed + 1;
    END IF;
    
    -- Registrar en auditoría
    INSERT INTO audit_logs (user_id, action, details)
    VALUES (p_user_id, 'store_purchase', 
            JSON_OBJECT('order_id', v_order_id, 'amount', v_final_amount));
    
    -- Retornar ID de la orden
    SELECT v_order_id AS order_id, v_order_number AS order_number, v_final_amount AS amount;
END$$

DELIMITER ;

-- ============================================
-- VISTAS ÚTILES
-- ============================================

-- Vista para estadísticas de usuarios
CREATE VIEW `user_statistics` AS
SELECT 
    u.id,
    u.username,
    u.level,
    u.experience,
    u.daily_login_streak,
    u.total_login_days,
    u.total_play_time,
    COUNT(DISTINCT uc.id) as character_count,
    COUNT(DISTINCT f.friend_id) as friend_count,
    COUNT(DISTINCT ua.achievement_id) as achievement_count,
    COUNT(DISTINCT ub.badge_id) as badge_count,
    COALESCE(SUM(so.final_amount), 0) as total_spent,
    MAX(uc.character_level) as highest_character_level
FROM launcher_users u
LEFT JOIN user_characters uc ON u.id = uc.user_id
LEFT JOIN friendships f ON u.id = f.user_id AND f.status = 'accepted'
LEFT JOIN user_achievements ua ON u.id = ua.user_id AND ua.completed = TRUE
LEFT JOIN user_badges ub ON u.id = ub.user_id
LEFT JOIN store_orders so ON u.id = so.user_id AND so.status = 'completed'
GROUP BY u.id;

-- Vista para eventos próximos
CREATE VIEW `upcoming_events` AS
SELECT 
    e.*,
    COUNT(ep.id) as registered_count,
    CASE 
        WHEN e.registration_start IS NULL OR e.registration_start <= NOW() THEN 'open'
        ELSE 'closed'
    END as registration_status,
    TIMESTAMPDIFF(MINUTE, NOW(), e.start_time) as minutes_until_start
FROM server_events e
LEFT JOIN event_participants ep ON e.id = ep.event_id AND ep.status = 'registered'
WHERE e.status IN ('scheduled', 'registration_open')
    AND e.end_time > NOW()
GROUP BY e.id
ORDER BY e.start_time ASC;

-- Vista para tickets pendientes
CREATE VIEW `pending_tickets` AS
SELECT 
    t.*,
    u.username as user_username,
    a.username as assigned_username,
    DATEDIFF(NOW(), t.created_at) as days_open,
    (SELECT COUNT(*) FROM ticket_replies tr WHERE tr.ticket_id = t.id) as reply_count
FROM support_tickets t
LEFT JOIN launcher_users u ON t.user_id = u.id
LEFT JOIN launcher_users a ON t.assigned_to = a.id
WHERE t.status IN ('open', 'in_progress')
ORDER BY 
    CASE t.priority
        WHEN 'critical' THEN 1
        WHEN 'high' THEN 2
        WHEN 'medium' THEN 3
        WHEN 'low' THEN 4
    END,
    t.created_at ASC;

-- Vista para notificaciones no leídas
CREATE VIEW `unread_notifications` AS
SELECT 
    n.*,
    u.username,
    TIMESTAMPDIFF(MINUTE, n.created_at, NOW()) as minutes_ago
FROM notifications n
JOIN launcher_users u ON n.user_id = u.id
WHERE n.is_read = FALSE
    AND (n.expires_at IS NULL OR n.expires_at > NOW())
ORDER BY n.created_at DESC;

-- ============================================
-- ÍNDICES ADICIONALES PARA OPTIMIZACIÓN
-- ============================================

-- Índices para búsquedas frecuentes
CREATE INDEX idx_private_messages_conversation_date 
ON private_messages(sender_id, receiver_id, created_at DESC);

CREATE INDEX idx_friendships_status_date 
ON friendships(status, created_at);

CREATE INDEX idx_store_orders_user_status 
ON store_orders(user_id, status, created_at DESC);

CREATE INDEX idx_user_achievements_user_completed 
ON user_achievements(user_id, completed, completed_at DESC);

CREATE INDEX idx_event_participants_event_status 
ON event_participants(event_id, status);

CREATE INDEX idx_server_statistics_date 
ON server_statistics(timestamp DESC);

-- ============================================
-- TRIGGERS PARA INTEGRIDAD DE DATOS
-- ============================================

DELIMITER $$

-- Trigger para actualizar experiencia automáticamente
CREATE TRIGGER `trg_update_user_level`
BEFORE UPDATE ON `launcher_users`
FOR EACH ROW
BEGIN
    DECLARE v_required_exp INT;
    DECLARE v_new_level INT;
    
    -- Calcular nivel basado en experiencia
    SET v_new_level = 1;
    
    WHILE v_new_level < 100 DO
        SET v_required_exp = 100 * v_new_level * v_new_level; -- Fórmula de experiencia
        
        IF NEW.experience < v_required_exp THEN
            LEAVE WHILE;
        END IF;
        
        SET v_new_level = v_new_level + 1;
    END WHILE;
    
    SET NEW.level = v_new_level - 1;
END$$

-- Trigger para limpiar notificaciones antiguas
CREATE TRIGGER `trg_cleanup_old_notifications`
BEFORE INSERT ON `notifications`
FOR EACH ROW
BEGIN
    -- Eliminar notificaciones leídas con más de 30 días
    DELETE FROM notifications 
    WHERE is_read = TRUE 
        AND created_at < DATE_SUB(NOW(), INTERVAL 30 DAY);
    
    -- Eliminar notificaciones no leídas con más de 90 días
    DELETE FROM notifications 
    WHERE is_read = FALSE 
        AND created_at < DATE_SUB(NOW(), INTERVAL 90 DAY);
END$$

-- Trigger para registrar cambios en usuarios
CREATE TRIGGER `trg_log_user_changes`
AFTER UPDATE ON `launcher_users`
FOR EACH ROW
BEGIN
    IF OLD.username != NEW.username THEN
        INSERT INTO audit_logs (user_id, action, details)
        VALUES (NEW.id, 'username_changed', 
                JSON_OBJECT('old', OLD.username, 'new', NEW.username));
    END IF;
    
    IF OLD.level != NEW.level THEN
        INSERT INTO audit_logs (user_id, action, details)
        VALUES (NEW.id, 'level_up', 
                JSON_OBJECT('old_level', OLD.level, 'new_level', NEW.level));
    END IF;
END$$

DELIMITER ;

-- ============================================
-- FINALIZAR CONFIGURACIÓN
-- ============================================

SELECT 'ThunderCore Launcher Pro database setup completed successfully!' AS message;

-- Mostrar resumen de tablas creadas
SELECT 
    TABLE_NAME as `Table`,
    TABLE_ROWS as `Rows`,
    ROUND(DATA_LENGTH/1024/1024, 2) as `Size (MB)`,
    ROUND(INDEX_LENGTH/1024/1024, 2) as `Index (MB)`,
    CREATE_TIME as `Created`
FROM information_schema.TABLES 
WHERE TABLE_SCHEMA = 'launcher'
ORDER BY TABLE_NAME;