import { Input } from "@/components/UI/input/Input"
import styles from "./FormInput.module.css"
import cls from "@/utils/cls"
import { Typography } from "../typography/Typography"

interface FormInputProps extends React.InputHTMLAttributes<HTMLInputElement> {
    invalidData?: boolean
    className?: string
    textOnInvalidData?: string
}

export const FormInput = ({
    className = "",
    textOnInvalidData = "Invalid data",
    invalidData,
    ...props
}: FormInputProps) => {
    return (
        <div className={styles.wrapper}>
            <Input
                className={cls(className, styles.form_input, invalidData ? styles.invalid : "")}
                {...props}
            />
            <Typography
                tag="span"
                variant="text_small"
                className={cls(styles.invalid_text, invalidData ? styles.on : "")}
            >
                {textOnInvalidData}
            </Typography>
        </div>
    )
}
