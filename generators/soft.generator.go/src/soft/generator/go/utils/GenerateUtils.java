package soft.generator.go.utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

import org.eclipse.emf.ecore.EClass;
import org.eclipse.emf.ecore.EClassifier;
import org.eclipse.emf.ecore.EPackage;

public class GenerateUtils {
    public List<EClass> getOrderedClasses(EPackage ePackage) {
        List<EClass> result = new ArrayList<EClass>();
        Set<EClass> resultSet = new HashSet<EClass>();
        for (EClassifier eClassifier : ePackage.getEClassifiers()) {
            if (eClassifier instanceof EClass) {
                EClass eClass = (EClass) eClassifier;
                List<EClass> extendChain = new LinkedList<>();
                Set<EClass> visited = new HashSet<EClass>();
                while (eClass != null && visited.add(eClass)) {
                    if (ePackage == eClass.getEPackage() && resultSet.add(eClass))
                        extendChain.add(0, eClass);

                    eClass = getSuperType(eClass);
                }
                result.addAll(extendChain);
            }
        }
        return result;
    }

    private EClass getSuperType(EClass eClass) {
        return eClass.getESuperTypes().stream().findFirst().orElse(null);
    }

    public String splitAndCombineWords(String s, String separator) {
        List<String> parsedName = new ArrayList<String>();
        if (s.length() != 0)
            parsedName.addAll(Arrays.asList(s.split("_")));

        StringBuilder result = new StringBuilder();
        for (Iterator<String> nameIter = parsedName.iterator(); nameIter.hasNext();) {
            String nameComponent = nameIter.next();
            result.append(nameComponent);

            if (nameIter.hasNext() && nameComponent.length() > 1)
                result.append(separator);
        }
        return result.toString();
    }
}
