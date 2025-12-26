-- Ejecutar en una nueva base de datos llamada 'launcher'
CREATE DATABASE IF NOT EXISTS `launcher` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE `launcher`;

-- Tabla de usuarios del launcher
CREATE TABLE `launcher_users` (
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `account_id` INT UNSIGNED NOT NULL UNIQUE,
    `username` VARCHAR(255) NOT NULL,
    `avatar_url` VARCHAR(500) DEFAULT NULL,
    `level` INT UNSIGNED DEFAULT 1,
    `experience` INT UNSIGNED DEFAULT 0,
    `daily_login_streak` INT UNSIGNED DEFAULT 0,
    `last_login_date` DATE DEFAULT NULL,
    `total_login_days` INT UNSIGNED DEFAULT 0,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX `idx_account` (`account_id`)
) ENGINE=InnoDB;

-- Tabla de mensajes privados
CREATE TABLE `private_messages` (
    `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `sender_id` INT UNSIGNED NOT NULL,
    `receiver_id` INT UNSIGNED NOT NULL,
    `subject` VARCHAR(255) NOT NULL,
    `message` TEXT NOT NULL,
    `read` BOOLEAN DEFAULT FALSE,
    `deleted_by_sender` BOOLEAN DEFAULT FALSE,
    `deleted_by_receiver` BOOLEAN DEFAULT FALSE,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX `idx_sender` (`sender_id`),
    INDEX `idx_receiver` (`receiver_id`),
    INDEX `idx_conversation` (`sender_id`, `receiver_id`),
    FOREIGN KEY (`sender_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE,
    FOREIGN KEY (`receiver_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de amistades
CREATE TABLE `friendships` (
    `user_id` INT UNSIGNED NOT NULL,
    `friend_id` INT UNSIGNED NOT NULL,
    `status` ENUM('pending', 'accepted', 'blocked') DEFAULT 'pending',
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (`user_id`, `friend_id`),
    INDEX `idx_user` (`user_id`),
    INDEX `idx_friend` (`friend_id`),
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE,
    FOREIGN KEY (`friend_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de notificaciones
CREATE TABLE `notifications` (
    `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `user_id` INT UNSIGNED NOT NULL,
    `type` ENUM('message', 'friend_request', 'event', 'reward', 'system') NOT NULL,
    `title` VARCHAR(255) NOT NULL,
    `content` TEXT NOT NULL,
    `read` BOOLEAN DEFAULT FALSE,
    `action_url` VARCHAR(500) DEFAULT NULL,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX `idx_user` (`user_id`),
    INDEX `idx_unread` (`user_id`, `read`),
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de logros del launcher
CREATE TABLE `launcher_achievements` (
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `code` VARCHAR(50) UNIQUE NOT NULL,
    `name` VARCHAR(100) NOT NULL,
    `description` TEXT NOT NULL,
    `points` INT UNSIGNED NOT NULL,
    `icon_url` VARCHAR(500) DEFAULT NULL,
    `category` VARCHAR(50) NOT NULL
) ENGINE=InnoDB;

-- Tabla de logros desbloqueados por usuarios
CREATE TABLE `user_achievements` (
    `user_id` INT UNSIGNED NOT NULL,
    `achievement_id` INT UNSIGNED NOT NULL,
    `unlocked_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `progress` INT UNSIGNED DEFAULT 0,
    `completed` BOOLEAN DEFAULT FALSE,
    PRIMARY KEY (`user_id`, `achievement_id`),
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE,
    FOREIGN KEY (`achievement_id`) REFERENCES `launcher_achievements`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de insignias/títulos
CREATE TABLE `badges` (
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `name` VARCHAR(100) NOT NULL,
    `description` TEXT,
    `icon_url` VARCHAR(500) DEFAULT NULL,
    `rarity` ENUM('common', 'uncommon', 'rare', 'epic', 'legendary') DEFAULT 'common',
    `requirements` TEXT
) ENGINE=InnoDB;

-- Tabla de insignias de usuarios
CREATE TABLE `user_badges` (
    `user_id` INT UNSIGNED NOT NULL,
    `badge_id` INT UNSIGNED NOT NULL,
    `equipped` BOOLEAN DEFAULT FALSE,
    `acquired_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`user_id`, `badge_id`),
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE,
    FOREIGN KEY (`badge_id`) REFERENCES `badges`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de actividad diaria
CREATE TABLE `daily_rewards` (
    `day` INT UNSIGNED NOT NULL,
    `reward_type` ENUM('points', 'badge', 'title', 'item') NOT NULL,
    `reward_value` VARCHAR(255) NOT NULL,
    `reward_quantity` INT UNSIGNED DEFAULT 1,
    `icon_url` VARCHAR(500) DEFAULT NULL,
    PRIMARY KEY (`day`)
) ENGINE=InnoDB;

-- Tabla de progreso diario
CREATE TABLE `daily_progress` (
    `user_id` INT UNSIGNED NOT NULL,
    `current_streak` INT UNSIGNED DEFAULT 0,
    `longest_streak` INT UNSIGNED DEFAULT 0,
    `last_claim_date` DATE DEFAULT NULL,
    `total_claimed` INT UNSIGNED DEFAULT 0,
    PRIMARY KEY (`user_id`),
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de tickets de soporte
CREATE TABLE `support_tickets` (
    `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `user_id` INT UNSIGNED NOT NULL,
    `category` VARCHAR(50) NOT NULL,
    `subject` VARCHAR(255) NOT NULL,
    `message` TEXT NOT NULL,
    `status` ENUM('open', 'in_progress', 'resolved', 'closed') DEFAULT 'open',
    `priority` ENUM('low', 'medium', 'high', 'critical') DEFAULT 'medium',
    `assigned_to` INT UNSIGNED DEFAULT NULL,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    `updated_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    INDEX `idx_user_status` (`user_id`, `status`),
    INDEX `idx_status` (`status`),
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de respuestas a tickets
CREATE TABLE `ticket_replies` (
    `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `ticket_id` BIGINT UNSIGNED NOT NULL,
    `user_id` INT UNSIGNED NOT NULL,
    `message` TEXT NOT NULL,
    `is_staff` BOOLEAN DEFAULT FALSE,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX `idx_ticket` (`ticket_id`),
    FOREIGN KEY (`ticket_id`) REFERENCES `support_tickets`(`id`) ON DELETE CASCADE,
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Tabla de eventos del servidor
CREATE TABLE `server_events` (
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `title` VARCHAR(255) NOT NULL,
    `description` TEXT,
    `event_type` VARCHAR(50) NOT NULL,
    `start_time` DATETIME NOT NULL,
    `end_time` DATETIME NOT NULL,
    `created_by` INT UNSIGNED DEFAULT NULL,
    `max_participants` INT UNSIGNED DEFAULT 0,
    `current_participants` INT UNSIGNED DEFAULT 0,
    `status` ENUM('scheduled', 'ongoing', 'completed', 'cancelled') DEFAULT 'scheduled',
    `reward_description` TEXT,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX `idx_time` (`start_time`, `end_time`),
    INDEX `idx_status` (`status`)
) ENGINE=InnoDB;

-- Tabla de participantes de eventos
CREATE TABLE `event_participants` (
    `event_id` INT UNSIGNED NOT NULL,
    `user_id` INT UNSIGNED NOT NULL,
    `character_guid` BIGINT UNSIGNED DEFAULT NULL,
    `status` ENUM('registered', 'attended', 'won', 'lost') DEFAULT 'registered',
    `registered_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`event_id`, `user_id`),
    FOREIGN KEY (`event_id`) REFERENCES `server_events`(`id`) ON DELETE CASCADE,
    FOREIGN KEY (`user_id`) REFERENCES `launcher_users`(`account_id`) ON DELETE CASCADE
) ENGINE=InnoDB;

-- Insertar logros predeterminados
INSERT INTO `launcher_achievements` (`code`, `name`, `description`, `points`, `category`) VALUES
('LOGIN_1', 'Primer Inicio', 'Inicia sesión por primera vez', 10, 'general'),
('LOGIN_7', 'Semana Activa', 'Inicia sesión 7 días seguidos', 50, 'general'),
('LOGIN_30', 'Mes Fiel', 'Inicia sesión 30 días seguidos', 200, 'general'),
('MSG_1', 'Primer Mensaje', 'Envía tu primer mensaje privado', 15, 'social'),
('FRIEND_1', 'Amistad Forjada', 'Añade a tu primer amigo', 20, 'social'),
('FRIEND_10', 'Popular', 'Consigue 10 amigos', 100, 'social'),
('PROFILE_COMPLETE', 'Perfil Completo', 'Completa tu perfil al 100%', 30, 'profile'),
('DAILY_CLAIM_7', 'Reclamante Semanal', 'Reclama recompensas diarias 7 días seguidos', 75, 'daily'),
('ACHIEVEMENT_10', 'Coleccionista', 'Desbloquea 10 logros', 150, 'achievements'),
('PARTICIPANT', 'Participante Activo', 'Participa en un evento del servidor', 40, 'events'),
('WINNER', 'Campeón', 'Gana un evento PvP', 100, 'pvp'),
('PVE_MASTER', 'Maestro PvE', 'Completa un raid en modo héroe', 120, 'pve'),
('LAUNCHER_LEVEL_10', 'Nivel 10', 'Alcanza el nivel 10 en el launcher', 200, 'level'),
('LAUNCHER_LEVEL_25', 'Nivel 25', 'Alcanza el nivel 25 en el launcher', 500, 'level'),
('LAUNCHER_LEVEL_50', 'Nivel 50', 'Alcanza el nivel 50 en el launcher', 1000, 'level');