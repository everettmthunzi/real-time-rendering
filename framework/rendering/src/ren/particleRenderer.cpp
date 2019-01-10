/******************************************************************************
 * Concept & Code from  Bartłomiej Filipek									  *
 * https://github.com/fenbf/particles 										  *
 * https://www.bfilipek.com/2014/03/three-particle-effects.html				  *
 ******************************************************************************/

#include <rtr/ren/particleRenderer.hpp>


void ParticleRenderer::generate(ParticleSystem *sys){
	//assert(sys != nullptr);
	m_sys = sys;
	const size_t count = sys->getCount();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_buf_pos);
	glBindBuffer(GL_ARRAY_BUFFER, m_buf_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 4 * count, nullptr, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, (4)*sizeof(float), (void *)((0)*sizeof(float)));
	
	glGenBuffers(1, &m_buf_col);
	glBindBuffer(GL_ARRAY_BUFFER, m_buf_col);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 4 * count, nullptr, GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (4)*sizeof(float), (void *)((0)*sizeof(float)));

	glGenBuffers(1, &m_buf_time);
	glBindBuffer(GL_ARRAY_BUFFER, m_buf_time);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 4 * count, nullptr, GL_STREAM_DRAW);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (4)*sizeof(float), (void *)((0)*sizeof(float)));
    if(m_sys->finalData()->m_indices->size() != 0){
      glGenBuffers(1, &m_buf_indices);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buf_indices);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sys->finalData()->m_indices->size() * sizeof(unsigned int),
                   m_sys->finalData()->m_indices->data(), GL_STATIC_DRAW);
    }


	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleRenderer::destroy() {
	if (m_buf_pos != 0)
	{
		glDeleteBuffers(1, &m_buf_pos);
		m_buf_pos = 0;
	}

	if (m_buf_col != 0)
	{
		glDeleteBuffers(1, &m_buf_col);
		m_buf_col = 0;
	}
}

void ParticleRenderer::update() {
	assert(m_sys);
	assert(m_buf_pos > 0 && m_buf_col > 0);

	const size_t count = m_sys->getAliveCount();
	if (count > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_buf_pos);
		float *ptr = (float *)(m_sys->finalData()->m_pos.get());
		glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(float)* 4, ptr);

		glBindBuffer(GL_ARRAY_BUFFER, m_buf_col);
		ptr = (float*)(m_sys->finalData()->m_col.get());
		glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(float)* 4, ptr);

		glBindBuffer(GL_ARRAY_BUFFER, m_buf_time);
		ptr = (float*)(m_sys->finalData()->m_time.get());
		glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(float)* 4, ptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ParticleRenderer::render() {
    const size_t count = m_sys->getAliveCount();


    if(m_sys->finalData()->m_indices->data() != 0){
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buf_indices);
      glBindVertexArray(m_vao);
      glDrawElements(GL_TRIANGLES, m_sys->finalData()->m_indices->size(), GL_UNSIGNED_INT, NULL);
      glBindVertexArray(0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else {

      if (count > 0)
        glBindVertexArray(m_vao);
        glDrawArrays(GL_LINE_STRIP, 0, count);
        glBindVertexArray(0);
    }



}