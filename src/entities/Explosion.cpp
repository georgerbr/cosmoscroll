#include "Explosion.hpp"
#include "EntityManager.hpp"
#include "core/SoundSystem.hpp"


Explosion::Explosion()
{
	m_animator.setAnimation(*this, EntityManager::getInstance().getAnimation("explosion"));
	SoundSystem::GetInstance().PlaySound("boom.ogg");

	setOrigin(getWidth() / 2, getHeight() / 2);
}


void Explosion::collides(Entity& entity)
{
	entity.onCollision(*this);
}


Explosion* Explosion::clone() const
{
	return new Explosion(*this);
}


void Explosion::onUpdate(float frametime)
{
	move(-EntityManager::FOREGROUND_SPEED * frametime, 0);

	m_animator.updateSubRect(*this, frametime);
	if (m_clock.getElapsedTime().asSeconds() > m_animator.getAnimation()->getDuration())
	{
		kill();
	}
}