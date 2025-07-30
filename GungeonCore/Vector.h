/**********************************************************************************
// Vector (Arquivo de Cabeçalho)
// 
// Criação:     18 Nov 2011
// Atualização: 20 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Classe para representar um vetor
//
**********************************************************************************/

#ifndef _PROGJOGOS_VECTOR_H_
#define _PROGJOGOS_VECTOR_H_

// --------------------------------------------------------------------------------

#include <cmath>

// --------------------------------------------------------------------------------

class Vector
{
private:
    static const double PI;             // constante PI
    float angle;                        // ângulo do vetor com o eixo x
    float magnitude;                    // magnitude do vetor

public:
    Vector();                           // construtor padrão
    Vector(float ang, float mag);       // construtor com ângulo e magnitude

    void Scale(float factor);           // amplia ou reduz vetor por um fator
    void ScaleTo(float value);          // ajusta rotação para valor indicado
    void Rotate(float theta);           // rotaciona vetor por ângulo em graus
    void RotateTo(float value);         // ajusta rotação para valor indicado
    void Add(const Vector& v);          // adiciona vetor recebido por parâmetro
    
    void SetFromCartesian(float x, float y);
    float Angle() const;                // retorna ângulo do vetor
    float Magnitude() const;            // retorna magnitude do vetor
    float XComponent() const;           // retorna componente X do vetor
    float YComponent() const;           // retorna componente Y do vetor
    float Radians() const;              // retorna ângulo em radianos
}; 

// ---------------------------------------------------------------------------------
// Funções Membro Inline

inline void Vector::SetFromCartesian(float x, float y)
{
    // Calcula a nova magnitude usando o Teorema de Pitágoras
    this->magnitude = sqrt(x * x + y * y);

    // Calcula o novo ângulo usando a arco-tangente de 2 argumentos
    // atan2 retorna em radianos, então convertemos para graus
    if (this->magnitude > 0) // Evita chamar atan2 para um vetor nulo
    {
        this->angle = atan2(y, x) * (180.0f / (float) PI);
    }
    // Se a magnitude for 0, o ângulo não importa, pode continuar como está.
}

inline void Vector::ScaleTo(float value)
{ magnitude = value; }

inline void Vector::RotateTo(float value)
{ angle = value; }

inline float Vector::Angle() const
{ return angle; }

inline float Vector::Magnitude() const
{ return magnitude; }

inline float Vector::XComponent() const
{ return magnitude * cos(Radians()); }    

inline float Vector::YComponent() const
{ return magnitude * sin(Radians()); }    

inline float Vector::Radians() const
{ return float(angle * PI / 180.0); }

// ------------------------------------------------------------------------------

#endif