import sys
import re
import io

# Forzamos la salida en UTF-8 para evitar errores en Windows con emojis
if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')

def filter_content(content):
    # Reemplazamos $...$ por \f$... \f$ para que Doxygen lo interprete como LaTeX
    # El regex busca contenido entre símbolos '$' que no contengan otro '$'
    # Usamos re.DOTALL para que funcione si hay saltos de línea (aunque no suele haber en inline)
    return re.sub(r'\$([^\$]+)\$', r'\\f$\1\\f$', content)

if __name__ == "__main__":
    # Doxygen pasa el nombre del archivo como primer argumento
    if len(sys.argv) > 1:
        try:
            with open(sys.argv[1], 'r', encoding='utf-8') as f:
                content = f.read()
                sys.stdout.write(filter_content(content))
        except Exception as e:
            # Si falla la lectura en UTF-8 o cualquier otra cosa, intentamos devolver el original
            try:
                with open(sys.argv[1], 'r', encoding='latin-1') as f:
                    sys.stdout.write(f.read())
            except:
                pass
    else:
        # Modo pipe
        content = sys.stdin.read()
        sys.stdout.write(filter_content(content))
