/******************************************************************************
 * Concept & Code from  Bartłomiej Filipek									  *
 * https://github.com/fenbf/particles 										  *
 * https://www.bfilipek.com/2014/03/three-particle-effects.html				  *
 ******************************************************************************/
#include <rtr/ren/particleEffects.hpp>

void FlameEffect::init(size_t numParticles) {
	m_sys = std::make_shared<ParticleSystem>(numParticles);
	//Generators
	m_posGen = std::make_shared<PointPosGen>(PointPosGen(glm::fvec4{0.f, 0.f, 0.f, 0.0f}));
	m_velGen = std::make_shared<SphereVelGen>(SphereVelGen(0.01f, 0.2f));
	m_colGen = std::make_shared<BasicColorGen>(BasicColorGen(glm::fvec4{0.5f, 0.02f, 0.005f, 0.0f}, 
											   				 glm::fvec4{0.8f, 0.1f, 0.006f, 0.0f}, 
											   				 glm::fvec4{0.2f, 0.0f, 0.0f, 0.0f},
											   	 			 glm::fvec4{0.3f, 0.0f, 0.0f, 0.0f}));
	m_timeGen = std::make_shared<VelTimeGen>(VelTimeGen(0.8f, 0.4f, glm::fvec4{0.0, 2.0, 0.0, 0.0}));
	//create emiiter
	auto emmit = std::make_shared<ParticleEmitter>(100.f);
	emmit->addGenerator(m_posGen);
	emmit->addGenerator(m_velGen);
	emmit->addGenerator(m_colGen);
	emmit->addGenerator(m_timeGen);
	//Updaters
	m_accUp = std::make_shared<BasicAccUpdater>(glm::fvec4{0.0f, 6.f, 0.0f, 0.0f});
	m_velUp = std::make_shared<BasicVelUpdater>();
	m_noiseUp = std::make_shared<NoiseVelocityUpdater>(0.6f, 2.0f);
	m_posUp = std::make_shared<BasicPosUpdater>();
	m_colUp = std::make_shared<BasicColorUpdater>();
	m_timeUp = std::make_shared<BasicTimeUpdater>();

	m_sys->addUpdater(m_accUp);
	m_sys->addUpdater(m_velUp);
	m_sys->addUpdater(m_noiseUp);
	m_sys->addUpdater(m_posUp);
	m_sys->addUpdater(m_colUp);
	m_sys->addUpdater(m_timeUp);

	m_sys->addEmitter(emmit);

}



void FlameEffect::update(float dt) {
	return;
}

void FlameThrowerEffect::init(size_t numParticles) {
	m_sys = std::make_shared<ParticleSystem>(numParticles);
	//Generators
	m_posGen = std::make_shared<PointPosGen>(PointPosGen(glm::fvec4{0.f, 0.f, 0.f, 0.0f}));
	m_velGen = std::make_shared<ConeVelGen>(ConeVelGen());
	m_colGen = std::make_shared<BasicColorGen>(BasicColorGen(glm::fvec4{0.5f, 0.02f, 0.005f, 0.0f}, 
											   				 glm::fvec4{0.8f, 0.1f, 0.006f, 0.0f}, 
											   				 glm::fvec4{0.2f, 0.0f, 0.0f, 0.0f},
											   	 			 glm::fvec4{0.3f, 0.0f, 0.0f, 0.0f}));
	m_timeGen = std::make_shared<GaussTimeGen>(GaussTimeGen(2.f, 0.7f));
	//create emiiter
	auto emmit = std::make_shared<ParticleEmitter>(100.f);
	emmit->addGenerator(m_posGen);
	emmit->addGenerator(m_velGen);
	emmit->addGenerator(m_colGen);
	emmit->addGenerator(m_timeGen);
	//Updaters
	m_accUp = std::make_shared<BasicAccUpdater>(glm::fvec4{0.0f, 6.f, 0.0f, 0.0f});
	m_velUp = std::make_shared<BasicVelUpdater>();
	m_noiseUp = std::make_shared<NoiseVelocityUpdater>(0.6f, 2.0f);
	m_posUp = std::make_shared<BasicPosUpdater>();
	m_colUp = std::make_shared<BasicColorUpdater>();
	m_timeUp = std::make_shared<BasicTimeUpdater>();

	m_sys->addUpdater(m_accUp);
	m_sys->addUpdater(m_velUp);
	m_sys->addUpdater(m_noiseUp);
	m_sys->addUpdater(m_posUp);
	m_sys->addUpdater(m_colUp);
	m_sys->addUpdater(m_timeUp);

	m_sys->addEmitter(emmit);

}



void FlameThrowerEffect::update(float dt) {
	return;
}

void FlameThrowerEffect::setPos(glm::fvec3 pos){
	m_posGen->m_pos = glm::fvec4(pos, 1.0);
}