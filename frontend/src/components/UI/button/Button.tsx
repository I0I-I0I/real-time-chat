import cls from "@/utils/cls"
import React from "react"
import styles from "./Button.module.css"

type ButtonVariants = "default" | "send" | "icon" | "link"

interface ButtonProps extends React.ButtonHTMLAttributes<HTMLButtonElement> {
    children?: string | React.ReactElement
    className?: string
    variant?: ButtonVariants
}

export const Button = ({
    children,
    className = "",
    variant = "default",
    ...props
}: ButtonProps): JSX.Element => (
    <button className={cls(styles.button, styles[variant], className)} {...props}>
        {children && children}
    </button>
)
