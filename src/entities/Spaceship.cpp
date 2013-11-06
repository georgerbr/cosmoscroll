#include "Spaceship.hpp"
#include "EntityManager.hpp"
#include "Player.hpp"

#include "utils/Math.hpp"
#include "utils/StringUtils.hpp"
#include "core/ParticleSystem.hpp"
#include "core/MessageSystem.hpp"

// bonus freq = 1 / DROP_LUCK
#define DROP_LUCK 8

// sinus movement settings
#define SINUS_AMPLITUDE 60
#define SINUS_FREQUENCE 0.02

// circle movement settings
#define CIRCLE_START_AT 400
#define CIRCLE_RADIUS   60
#define CIRCLE_ROTATION_SPEED (math::PI * 0.8)

Spaceship::Spaceship(const Animation& animation, int hp, int speed):
	m_attack(NONE),
	m_movement(LINE),
	m_points(1),
	m_target(NULL)
{
	setTexture(animation.getTexture());
	setTeam(Entity::BAD);
	setHP(hp);
	m_animator.setAnimation(*this, animation);

	speed_ = speed;
	m_weapon.setOwner(this);
	base_y_ = 0;
	base_x_ = 0;
	angle_ = 2 * math::PI;
}


Spaceship::~Spaceship()
{
}


void Spaceship::onInit()
{
	if (m_movement == SINUS)
	{
		float y = getY();
		// Compute maximal Y position according to the wave's amplitude
		// (So the spacship is always within the screen's boundaries)
		float y_max = EntityManager::getInstance().getHeight() - SINUS_AMPLITUDE - getHeight();
		if (y < SINUS_AMPLITUDE)
			base_y_ = SINUS_AMPLITUDE;
		else if (y > y_max)
			base_y_ = y_max;
		else
			base_y_ = y;

		setY(base_y_);
	}
	else if (m_movement == CIRCLE)
	{
		float y = getY();
		float y_max = EntityManager::getInstance().getHeight() - CIRCLE_RADIUS - getHeight();
		if (y < CIRCLE_RADIUS)
			base_y_ = CIRCLE_RADIUS;
		else if (y > y_max)
			base_y_ = y_max;
		else
			base_y_ = y;

		setY(base_y_);
	}

	m_target = EntityManager::getInstance().GetPlayerShip();
}


void Spaceship::setMovementPattern(MovementPattern movement)
{
	m_movement = movement;
}


void Spaceship::setAttackPattern(AttackPattern attack)
{
	m_attack = attack;
}


Spaceship* Spaceship::clone() const
{
	Spaceship* ship = new Spaceship(*this);
	ship->setPoints(getPoints());
	ship->m_weapon.setOwner(ship);
	return ship;
}


void Spaceship::onUpdate(float frametime)
{
	m_animator.updateSubRect(*this, frametime);

	// Apply movement pattern
	float delta = speed_ * frametime;
	switch (m_movement)
	{
		case LINE:
		{
			move(-delta, 0);
		}
		break;
		case MAGNET:
		{
			sf::Vector2f player_pos = m_target->getCenter();
			sf::Vector2f my_pos = getCenter();

			float x_diff = my_pos.x - player_pos.x;
			float y_diff = my_pos.y - player_pos.y;

			if (x_diff > 5)
				move(-delta, 0);
			else if (x_diff < -5)
				move(delta, 0);

			if (y_diff > 5)
				move(0, -delta);
			else if (y_diff < -5)
				move(0, delta);
		}
		break;
		case SINUS:
		{
			sf::Vector2f pos = getPosition();
			pos.x += -speed_ * frametime;
			pos.y = std::sin(pos.x * SINUS_FREQUENCE) * SINUS_AMPLITUDE + base_y_;
			setPosition(pos);
		}
		break;
		case CIRCLE:
		{
			if (getX() > CIRCLE_START_AT + CIRCLE_RADIUS)
			{
				move(-delta, 0);
			}
			else
			{
				sf::Vector2f pos(CIRCLE_START_AT, base_y_);
				angle_ += CIRCLE_ROTATION_SPEED * frametime;
				pos.x += CIRCLE_RADIUS * std::cos(angle_);
				pos.y -= CIRCLE_RADIUS * std::sin(angle_);
				setPosition(pos);
			}
		}
		break;
	}

	switch (m_attack)
	{
		case AUTO_AIM:
			m_weapon.shoot(m_target->getCenter());
			break;

		case ON_SIGHT:
		{
			float my_y = getCenter().y;
			float player_y = m_target->getCenter().y;
			// if both Spaceships are roughly on the same Y axis
			if (std::abs(player_y - my_y) < 30)
			{
				m_weapon.shoot(-math::PI);
			}
		}
			break;
		case NONE:
			break;
	}

	m_weapon.onUpdate(frametime);

	updateDamageFlash(frametime);
}


void Spaceship::onDestroy()
{
	Damageable::onDestroy();
	if (math::random(1, DROP_LUCK) == 1)
	{
		PowerUp::dropRandom(getPosition());
	}

	EntityManager::getInstance().GetPlayerShip()->updateScore(m_points);
	MessageSystem::write("+" + std::to_string(m_points), getPosition(), sf::Color(255, 128, 0));
}


void Spaceship::setPoints(int points)
{
	m_points = points;
}


int Spaceship::getPoints() const
{
	return m_points;
}
